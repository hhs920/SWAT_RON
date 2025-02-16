// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/CombatComponent.h"

#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터에 소켓들이 추가되어 있어야 한다.
	RightHandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	
	SetUpEquipments();

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SetUpEquipments()
{
	Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	Primary->SetEquipmentType(EEquipmentType::Primary);

	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
	Secondary->SetEquipmentType(EEquipmentType::Secondary);
	
	Equip(Primary);
    Equip(Secondary);
	
    HoldEquipment(Primary);
}

void UCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	UnholdEquipment(); 	// 들고있던 무기(EquippedWeapon)를 돌려놓는다.
	HoldEquipment(Equipment);
}

void UCombatComponent::Fire()
{
	//EquippedWeapon->
}

void UCombatComponent::Equip(AEquipment* Equipment)
{
	if ( !Equipment ) return;

	if (Equipment->OwnerCharacter == nullptr) 
		Equipment->OwnerCharacter = Character;

	USkeletalMeshSocket const* socketToEquipped = Equipment->GetSocketToEquipped();
	if (socketToEquipped)
	{
		socketToEquipped->AttachActor(Equipment, Character->GetMesh());
		Equipment->SetEquippedState(EEquippedState::Equipped);

	}
}

void UCombatComponent::PickUpWeapon(class AWeapon* WeaponToEquip)
{

	
	// if (Character == nullptr || WeaponToEquip == nullptr)
	// 	return;
	// switch (WeaponToEquip->GetEquipmentType())
	// {
	// case EEquipmentType::Primary:
	// 	{
	// 		Primary = WeaponToEquip;
	// 	}
	// 	break;
	// case EEquipmentType::Secondary:
	// 	{
	// 		Secondary = WeaponToEquip;
	// 	}
	// 	break;
	// }
	// WeaponToEquip->SetEquippedState(EEquippedState::Equipped);
	//
	// RightHandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	// if (RightHandSocket)
	// {
	// 	RightHandSocket->AttachActor(WeaponToEquip, Character->GetMesh());
	// }
}

void UCombatComponent::UnholdEquipment()
{
	if (Character == nullptr || Character->GetMesh() == nullptr || HoldingEquipment == nullptr)
		return;
	
	USkeletalMeshSocket const* socketToEquipped = HoldingEquipment->GetSocketToEquipped();
	if (socketToEquipped)
	{
		socketToEquipped->AttachActor(HoldingEquipment, Character->GetMesh());
		HoldingEquipment->SetEquippedState(EEquippedState::Equipped);
		HoldingEquipment = nullptr;
	}
}

void UCombatComponent::HoldEquipment(AEquipment* ToHandle)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ToHandle == nullptr)
		return;
	
	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(ToHandle, Character->GetMesh());
		HoldingEquipment = ToHandle;
		HoldingEquipment->SetEquippedState(EEquippedState::Holding);
	}
}


