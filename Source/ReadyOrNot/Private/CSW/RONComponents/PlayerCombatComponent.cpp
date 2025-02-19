// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/PlayerCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"

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
}

void UPlayerCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	Super::SwapEquipment(Equipment);
}

void UPlayerCombatComponent::FireWeaponSetTimer(AWeapon* holdingWeapon)
{
	// 총 발사
	GetWorld()->GetTimerManager().SetTimer(
		holdingWeapon->FireTimer,
		[this, holdingWeapon](){
			this->HoldingEquipment->BeginUse();
			this->PlayFireMontage(this->bAiming);
			holdingWeapon->Fire();
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
			break;
		case ESelectorState::Burst:
			{
				if (holdingWeapon->GetUsing())
					return;
				
				FireWeaponSetTimer(holdingWeapon);
			}
			break;
		case ESelectorState::FullAuto:
			{
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

void UPlayerCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UPlayerCombatComponent::InterpFOV(float DeltaTime)
{
	AWeapon* weapon = Cast<AWeapon>(HoldingEquipment);
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, weapon->GetZoomedFOV(),
			DeltaTime, weapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV,
			DeltaTime, weapon->GetZoomInterpSpeed());
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
