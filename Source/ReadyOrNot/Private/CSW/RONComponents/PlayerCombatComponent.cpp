// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/PlayerCombatComponent.h"

#include "ReadyOrNot.h"
#include "Camera/CameraComponent.h"
#include "CSW/Evidence.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Character/RONPlayerController.h"
#include "CSW/HUD/RONPlayerHUD.h"
#include "CSW/Weapon/Weapon.h"
#include "Editor/PropertyEditorTestObject.h"
#include "Kismet/GameplayStatics.h"

UPlayerCombatComponent::UPlayerCombatComponent() : UCombatComponent()
{
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

	if (PlayerCharacter)
	{
		// LineTrace
		FHitResult HitResult;
		TraceUnderCrossHairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		// HUD
		SetHudCrosshairs(DeltaTime);
	}

	if (GetWorld() && GEngine && GEngine->GameViewport)
		TraceForEvidence();

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

void UPlayerCombatComponent::TraceForEvidence()
{
	if (!GetWorld() || !GEngine || !GEngine->GameViewport) return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ViewportCenter = ViewportSize * 0.5f; // 화면 중앙 좌표

	// 화면 중앙의 위치를 3D World 좌표로 변환
	FVector WorldLocation {FVector()}, WorldDirection {FVector()};
	if (Controller)
	{
		Controller->DeprojectScreenPositionToWorld(ViewportCenter.X, ViewportCenter.Y, WorldLocation, WorldDirection);
	}

	// 라인 트레이스
	FVector TraceStart = WorldLocation;
	FVector TraceEnd = TraceStart + (WorldDirection * 150.f); // 1.5m
	FHitResult HitResult;

	// 충돌 정보 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this->Character); // 플레이어 자신은 무시

	// 레이 트레이스 실행
	// ECC_EngineTraceChannel4 ) Interact
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Interact, QueryParams);
    
	if (bHit && HitResult.GetActor()) 
	{
		// 맞은 오브젝트가 상호작용 대상이면
		IInteractable* interactable = Cast<IInteractable>(HitResult.GetActor());
		if (interactable)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("%s"), *HitResult.GetActor()->GetName()));
			// UI에 상호작용 메시지 표시 (예: "Press E to Collect Evidence")
			//ShowInteractionUI(true, interactable);

			if (interacting != interactable)
			{
				auto weapon = Cast<AWeapon>(interacting);
				if (weapon)
				{
					weapon->ShowGatherEvidenceWidget(false);
				}
				auto evidence = Cast<AEvidence>(interacting);
				if (evidence)
				{
					evidence->ShowGatherEvidenceWidget(false);
				}
				
				auto newWeapon = Cast<AWeapon>(interactable);
				if (newWeapon)
				{
					newWeapon->ShowGatherEvidenceWidget(true);
				}
				auto newEvidence = Cast<AEvidence>(interactable);
				if (newEvidence)
				{
					newEvidence->ShowGatherEvidenceWidget(true);
				}
			}
			
			interacting = interactable; // 현재 보고 있는 증거
			return;
		}
	}

	// 아무것도 감지하지 못하면 UI 숨김
	if (interacting)
	{
		auto newWeapon = Cast<AWeapon>(interacting);
		if (newWeapon)
		{
			newWeapon->ShowGatherEvidenceWidget(false);
		}
		auto newEvidence = Cast<AEvidence>(interacting);
		if (newEvidence)
		{
			newEvidence->ShowGatherEvidenceWidget(false);
		}
	}
	interacting = nullptr;
}

void UPlayerCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	Super::SwapEquipment(Equipment);
}

void UPlayerCombatComponent::SetUpEquipments()
{
	Super::SetUpEquipments();

	if (SecondaryWeaponClass)
	{
		Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
		Secondary->SetEquipmentType(EEquipmentType::Secondary);
		Equip(Secondary);

		// 주무기가 없으면 보조무기를 든다.
		if (!Primary)
			HoldEquipment(Secondary);
	}

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
		// LineTrace의 시작 위치를 카메라가 아닌 적합한 위치로 세팅한다. 카메라가 아닌, 총에서 쏘는 것이기 때문이다.
		if(PlayerCharacter)
		{
			float DistToCharacter = (PlayerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistToCharacter + 100.f);  
			//DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::Red, false);
		}
		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		
		if (!TraceHitResult.bBlockingHit)  // 적중안하면
		{
			
			TraceHitResult.ImpactPoint = End;
			HoldingEquipment->LineTraceTarget = End;
		}
		else // 적중하면
		{
			HoldingEquipment->LineTraceTarget = End;
			// 디버그 - 충돌 지점에 구 그리기
			//DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f, 12, FColor::Red);
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

void UPlayerCombatComponent::GatherEvidence(IInteractable* ToInteract)
{
	ToInteract->BeginInteract();
	ToInteract->EndInteract();
}
