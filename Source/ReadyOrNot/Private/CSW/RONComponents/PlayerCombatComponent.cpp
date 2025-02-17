// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/PlayerCombatComponent.h"

#include "Camera/CameraActor.h"
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
		if (PlayerCharacter->GetCamera())
		{
			DefaultFOV = PlayerCharacter->GetCamera()->GetCameraComponent()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UPlayerCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	Super::SwapEquipment(Equipment);
}

void UPlayerCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (PlayerCharacter)
	{
		if (bFireButtonPressed)
		{
			PlayerCharacter->PlayFireMontage(bAiming);
		}
		else
		{
			// 총기가 연사모드인지 단발모드인지 점사모드인지 알아야한다.
		}
	}
}

void UPlayerCombatComponent::SetUpEquipments()
{
	Super::SetUpEquipments();
	
	Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
	Grenade->SetEquipmentType(EEquipmentType::Grenade);
	Equip(Grenade);

	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	Tactical->SetEquipmentType(EEquipmentType::Tactical);
	Equip(Tactical);

	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	LongTactical->SetEquipmentType(EEquipmentType::LongTactical);
	Equip(LongTactical);

	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);
	CableTie->SetEquipmentType(EEquipmentType::CableTie);
	Equip(CableTie);
}

void UPlayerCombatComponent::Fire()
{
	Super::Fire();
}


void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacter && PlayerCharacter->GetCamera())
	{
		InterpFOV(DeltaTime);
	}
}

void UPlayerCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (bAiming)
	{
		// 무기에 달린 카메라로 Set View Target with Blend 처리
		
	}
	else
	{
		// 플레이어에 달린 카메라로 Set View Target with Blend 처리
		
	}
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

	// if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	// {
	// 	PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV); 
	// }
	if (PlayerCharacter && PlayerCharacter->GetCamera())
	{
		PlayerCharacter->GetCamera()->GetCameraComponent()->SetFieldOfView(CurrentFOV); 
	}
}

// void UPlayerCombatComponent::GatherEvidence(class AWeapon* EvidenceToGather)
// {
// 	if (Character == nullptr || EvidenceToGather == nullptr)
// 		return;
// 	
// 	EvidenceToGather->SetWeaponState(EWeaponState::EWS_Gathered);
// }
