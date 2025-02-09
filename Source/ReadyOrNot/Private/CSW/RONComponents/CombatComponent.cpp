// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/CombatComponent.h"

#include "Components/SphereComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpEquipments();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetUpEquipments()
{
	Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
	Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);

}

void UCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	if (PlayerCharacter == nullptr || WeaponToEquip == nullptr)
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

	const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(WeaponToEquip, PlayerCharacter->GetMesh());
	}
}

void UCombatComponent::GatherEvidence(class AWeapon* EvidenceToGather)
{
	if (PlayerCharacter == nullptr || EvidenceToGather == nullptr)
		return;
	
	EvidenceToGather->SetWeaponState(EWeaponState::EWS_Gathered);
}




