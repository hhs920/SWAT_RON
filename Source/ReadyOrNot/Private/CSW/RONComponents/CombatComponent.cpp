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
	
	// 디폴트로 주무기를 착용중
	EquippedWeapon = Primary;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCombatComponent::SetUpEquipments()
{
	Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	Primary->SetWeaponType(EEquipmentType::EET_Primary);
	
	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
	Secondary->SetWeaponType(EEquipmentType::EET_Secondary);
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
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
}
