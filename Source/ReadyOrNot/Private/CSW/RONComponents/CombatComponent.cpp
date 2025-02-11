// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpEquipments();
	
	if (Character)
	{
		// DefaultFOV 세팅
		if (Character->GetFollowCamera())
		{
			DefaultFOV = Character->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->GetFollowCamera())
	{
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::SetUpEquipments()
{
	Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	Primary->SetWeaponType(EEquipmentType::EET_Primary);
	
	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
	Secondary->SetWeaponType(EEquipmentType::EET_Secondary);

	Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
	Grenade->SetWeaponType(EEquipmentType::EET_Grenade);

	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	Tactical->SetWeaponType(EEquipmentType::EET_Tactical);
	
	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	LongTactical->SetWeaponType(EEquipmentType::EET_LongTactical);

	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);
	CableTie->SetWeaponType(EEquipmentType::EET_CableTie);
	
	Character->SetEquippedWeapon(Primary);

	// TODO : 플레이어 캐릭터의 장비로 만들기, 안보이게 하기
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(),
			DeltaTime, EquippedWeapon->ZoomInterpSpeed);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV,
			DeltaTime, EquippedWeapon->ZoomInterpSpeed);
	}

	if (Character && Character->GetFollowCamera())
	{
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV); 
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (Character)
	{
		Character->PlayFireMontage(bAiming);
	}
}

void UCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
		return;
	switch (WeaponToEquip->GetEquipmentType())
	{
	case EEquipmentType::EET_Primary:
		{
			Primary = WeaponToEquip;
		}
		break;
	case EEquipmentType::EET_Secondary:
		{
			Secondary = WeaponToEquip;
		}
		break;
	case EEquipmentType::EET_Grenade:
		{
			Grenade = WeaponToEquip;
		}
		break;
	case EEquipmentType::EET_Tactical:
		{
			Tactical = WeaponToEquip;
		}
		break;
	case EEquipmentType::EET_LongTactical:
		{
			LongTactical = WeaponToEquip;
		}
		break;
	case EEquipmentType::EET_CableTie:
		{
			CableTie = WeaponToEquip;
		}
		break;
	}
	WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(WeaponToEquip, Character->GetMesh());
	}
}

void UCombatComponent::GatherEvidence(class AWeapon* EvidenceToGather)
{
	if (Character == nullptr || EvidenceToGather == nullptr)
		return;
	
	EvidenceToGather->SetWeaponState(EWeaponState::EWS_Gathered);
}




