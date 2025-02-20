// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/PlayerCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Character/RONPlayerController.h"
#include "CSW/HUD/RONPlayerHUD.h"
#include "CSW/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

UPlayerCombatComponent::UPlayerCombatComponent() : UCombatComponent()
{
}

void UPlayerCombatComponent::Interact(AActor* ToInteract)
{
	// F버튼으로 Interact
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCharacter)
	{
		// DefaultFOV 세팅
		if (PlayerCharacter->GetFollowCamera())
		{
			DefaultFOV = PlayerCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		InterpFOV(DeltaTime);
	}

	// LineTrace
	FHitResult HitResult;
	TraceUnderCrossHairs(HitResult);

	// HUD
	SetHudCrosshairs(DeltaTime);
}

void UPlayerCombatComponent::SetHudCrosshairs(float DeltaTime)
{
	if (PlayerCharacter == nullptr || PlayerCharacter->Controller == nullptr) return;

	Controller = (Controller == nullptr) ?
		Cast<ARONPlayerController>(PlayerCharacter->GetController()) :
		Controller;

	if (Controller)
	{
		HUD = (HUD == nullptr) ? Cast<ARONPlayerHUD>(Controller->GetHUD()) : HUD;

		if (HUD) // 크로스헤어 세팅
		{
			if (HoldingEquipment)
			{
				HUD->SetHUDPackage(HoldingEquipment->HUDPackage);
			}
			else
			{
				FHUDPackage HUDPackage;
				HUDPackage.CrosshairsCenter = nullptr;
				HUD->SetHUDPackage(HUDPackage);

			}
		}
	}
	
}

void UPlayerCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	Super::SwapEquipment(Equipment);
}

void UPlayerCombatComponent::SetUpEquipments()
{
	Super::SetUpEquipments();

	if (GrenadeWeaponClass)
	{
		Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
		Grenade->SetEquipmentType(EEquipmentType::Grenade);
		Equip(Grenade);
	}

	if (TacticalWeaponClass)
	{
		Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
		Tactical->SetEquipmentType(EEquipmentType::Tactical);
		Equip(Tactical);
	}

	if (LongTacticalWeaponClass)
	{
		LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
		LongTactical->SetEquipmentType(EEquipmentType::LongTactical);
		Equip(LongTactical);
	}

	if (CableTieWeaponClass)
	{
		CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);
		CableTie->SetEquipmentType(EEquipmentType::CableTie);
		Equip(CableTie);
	}

}

void UPlayerCombatComponent::FireWeaponSetTimer(AWeapon* holdingWeapon)
{
	// 총 발사
	GetWorld()->GetTimerManager().SetTimer(
		holdingWeapon->FireTimer,
		[this, holdingWeapon](){
			this->HoldingEquipment->BeginUse();
			this->PlayFireMontage(this->bAiming);
			//holdingWeapon->Fire(HitTarget);
		},
		holdingWeapon->GetFireDelay(),
		true
	);
}

void UPlayerCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (HoldingEquipment == nullptr) return;
	if (PlayerCharacter == nullptr) return; 
	
	// 총기일 때
	AWeapon* holdingWeapon = Cast<AWeapon>(HoldingEquipment);
	if (holdingWeapon == nullptr) return;
	
	if (bFireButtonPressed) // 누를 때
	{

		
		// 조정간 상태에 따른 처리
		switch (holdingWeapon->GetSelectorState())
		{
		case ESelectorState::SemiAuto:
			holdingWeapon->BeginUse(); // 한발 쏜다.
			PlayFireMontage(bAiming);
			//holdingWeapon->Fire(HitTarget);
			break;
		case ESelectorState::Burst:
			{
				if (holdingWeapon->GetUsing())
					return;

				// 첫 한발 쏜다.
				holdingWeapon->BeginUse(); 
				PlayFireMontage(bAiming);
				//holdingWeapon->Fire(HitTarget);
				holdingWeapon->SetBurstFireCount(1);
				
				FireWeaponSetTimer(holdingWeapon);
			}
			break;
		case ESelectorState::FullAuto:
			{
				// 첫 한발 쏜다.
				holdingWeapon->BeginUse(); 
				PlayFireMontage(bAiming);
				//holdingWeapon->Fire(HitTarget);
				
				FireWeaponSetTimer(holdingWeapon);
			}
			break;
		}
	}
	else // 뗄 때
	{
		HoldingEquipment->EndUse();
		
		switch (holdingWeapon->GetSelectorState())
		{
			case ESelectorState::FullAuto:
				GetWorld()->GetTimerManager().ClearTimer(holdingWeapon->FireTimer);
				break;
		}
	}
}

void UPlayerCombatComponent::TraceUnderCrossHairs(FHitResult& TraceHitResult)
{
	// 화면 중앙에서
	FVector2D ViewportSize { FVector2d() };
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrossharLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

	// Screen Location To World Location
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossharLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		// LineTrace
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		
		if (!TraceHitResult.bBlockingHit) 
		{
			// 적중안하면
			TraceHitResult.ImpactPoint = End;
			HoldingEquipment->LineTraceTarget = End;
		}
		else
		{
			// 적중하면
			HoldingEquipment->LineTraceTarget = End;
			// 디버그 - 충돌 지점에 구 그리기
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f,
				12, FColor::Red);
		}
	}
}

void UPlayerCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UPlayerCombatComponent::InterpFOV(float DeltaTime)
{
	AWeapon* Weapon = Cast<AWeapon>(HoldingEquipment);
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, Weapon->GetZoomedFOV(),
			DeltaTime, Weapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV,
			DeltaTime, Weapon->GetZoomInterpSpeed());
	}

	if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV); 
	}
}

// void UPlayerCombatComponent::GatherEvidence(class AWeapon* EvidenceToGather)
// {
// 	if (Character == nullptr || EvidenceToGather == nullptr)
// 		return;
// 	
// 	EvidenceToGather->SetWeaponState(EWeaponState::EWS_Gathered);
// }
