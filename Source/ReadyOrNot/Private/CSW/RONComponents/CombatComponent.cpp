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
	PrimarySocket = Character->GetMesh()->GetSocketByName(FName("PrimarySocket"));
	SecondarySocket = Character->GetMesh()->GetSocketByName(FName("SecondarySocket"));
	RightHandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	
	SetUpEquipments();
	SetUpInitialEquippedWeapon();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SetUpEquipments()
{
	Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);

	Primary->SetWeaponState(EWeaponState::EWS_Equipped);
	Secondary->SetWeaponState(EWeaponState::EWS_Equipped);
	
	Primary->SetWeaponType(EEquipmentType::EET_Primary);
	Secondary->SetWeaponType(EEquipmentType::EET_Secondary);

	// 캐릭터의 소켓에 부착한다.
	if (PrimarySocket)
		PrimarySocket->AttachActor(Primary, Character->GetMesh());

	if (SecondarySocket)
		SecondarySocket->AttachActor(Secondary, Character->GetMesh());
}

void UCombatComponent::SetUpInitialEquippedWeapon()
{
	// 캐릭터에 RightHandSocket이 있어야한다.
	if (RightHandSocket)
	{
		if (Primary)
		{
			// 주무기가 있으면 주무기를 처음에 들고있는다.
			RightHandSocket->AttachActor(Primary, Character->GetMesh());
			EquippedWeapon = Primary;
		}
		else if (Secondary)
		{
			// 보조무기가 있으면 보조무기를 처음에 들고있는다.
			RightHandSocket->AttachActor(Secondary, Character->GetMesh());
			Secondary->GetWeaponMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
			EquippedWeapon = Secondary;
		}
	}
}

void UCombatComponent::ChangeEquipment(EEquipmentType Type)
{
	// 기존에 들고있던 무기를 EquipmentType에 맞는 소켓에 돌려놓는다(Attach).
	switch (EquippedWeapon->GetEquipmentType())
	{
	case EEquipmentType::EET_Primary:
		{
			if (PrimarySocket)
			{
				PrimarySocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	case EEquipmentType::EET_Secondary:
		{
			if (SecondarySocket)
			{
				SecondarySocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	}

	// 바꿔들을 무기를 RightHandSocket에 Attach한다.
	switch (Type)
	{
	case EEquipmentType::EET_Primary:
		{
			if (PrimarySocket)
			{
				PrimarySocket->AttachActor(Primary, Character->GetMesh());
				RightHandSocket->AttachActor(Primary, Character->GetMesh());
				EquippedWeapon = Primary;
			}
		}
		break;
	case EEquipmentType::EET_Secondary:
		{
			if (SecondarySocket)
			{
				SecondarySocket->AttachActor(Secondary, Character->GetMesh());
				RightHandSocket->AttachActor(Secondary, Character->GetMesh());
				Secondary->GetWeaponMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
				EquippedWeapon = Secondary;
			}
		}
		break;
	}
}

// void UCombatComponent::AttachEquipmentToSocket(EEquipmentType EquipmentType)
// {
// }

void UCombatComponent::Fire()
{
	//EquippedWeapon->
}


// void UCombatComponent::PickUpWeapon(class AWeapon* WeaponToEquip)
// {
// 	if (Character == nullptr || WeaponToEquip == nullptr)
// 		return;
// 	switch (WeaponToEquip->GetEquipmentType())
// 	{
// 	case EEquipmentType::EET_Primary:
// 		{
// 			Primary = WeaponToEquip;
// 		}
// 		break;
// 	case EEquipmentType::EET_Secondary:
// 		{
// 			Secondary = WeaponToEquip;
// 		}
// 		break;
// 	}
// 	WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);
//
// 	HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
// 	if (HandSocket)
// 	{
// 		HandSocket->AttachActor(WeaponToEquip, Character->GetMesh());
// 	}
// }




