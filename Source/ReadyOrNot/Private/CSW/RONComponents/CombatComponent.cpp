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
	if (PrimaryWeaponClass)
	{
		Primary = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
		Primary->SetEquipmentType(EEquipmentType::Primary);
		Equip(Primary);
		HoldEquipment(Primary);
	}

	if (SecondaryWeaponClass)
	{
		Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
		Secondary->SetEquipmentType(EEquipmentType::Secondary);
		Equip(Secondary);

	}
}

void UCombatComponent::SwapEquipment(class AEquipment* Equipment)
{
	UnholdEquipment(); 	// 들고있던 무기(EquippedWeapon)를 돌려놓는다.
	HoldEquipment(Equipment);
}


void UCombatComponent::Equip(AEquipment* Equipment)
{
	if ( !Equipment ) return;

	if (Equipment->OwnerCharacter == nullptr)
	{
		Equipment->OwnerCharacter = Character;
		Equipment->SetOwner(Character);
	}
	USkeletalMeshSocket const* socketToEquipped = Equipment->GetSocketToEquipped();
	if (socketToEquipped)
	{
		socketToEquipped->AttachActor(Equipment, Character->GetMesh());
		if (Equipment->bRotateYtoX)
		{
			FRotator RotationOffset(0.f, -90.f, 0.f); // Y축을 X축으로 회전 (Yaw -90도)
			Equipment->SetActorRelativeRotation(RotationOffset);
		}
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

// hold중이던 장비를 소켓으로 되돌려놓는다.
void UCombatComponent::UnholdEquipment()
{
	if (Character == nullptr || Character->GetMesh() == nullptr || HoldingEquipment == nullptr)
		return;
	
	HoldingEquipment->Unequip();
	
	USkeletalMeshSocket const* socketToEquipped = HoldingEquipment->GetSocketToEquipped();
	if (socketToEquipped)
	{
		socketToEquipped->AttachActor(HoldingEquipment, Character->GetMesh());
		if (HoldingEquipment->bRotateYtoX)
		{
			FRotator RotationOffset(0.f, -90.f, 0.f); // Y축을 X축으로 회전 (Yaw -90도)
			HoldingEquipment->SetActorRelativeRotation(RotationOffset);
		}
		HoldingEquipment->SetEquippedState(EEquippedState::Equipped);
		HoldingEquipment = nullptr;
	}
}

// equip 중이던 장비를 righthand 소켓으로 가져온다.
void UCombatComponent::HoldEquipment(AEquipment* ToHandle)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ToHandle == nullptr)
		return;
	
	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(ToHandle, Character->GetMesh());
		if (ToHandle->bRotateYtoX)
		{
			FRotator RotationOffset(0.f, -90.f, 0.f); // Y축을 X축으로 회전 (Yaw -90도)
			ToHandle->SetActorRelativeRotation(RotationOffset);
		}
		ToHandle->Equip();
		HoldingEquipment = ToHandle;
		HoldingEquipment->SetEquippedState(EEquippedState::Holding);
	}
}

void UCombatComponent::PlayFireMontage(bool bAim)
{
	if (HoldingEquipment == nullptr)
		return;
	
	UAnimInstance* animInstance = Character->GetMesh()->GetAnimInstance();
	// Aim(줌인) 상태와 Ironsights(Assault 스탠스)일 때 다른 애니메이션을 출력한다.
	if (animInstance && FireWeaponMontage)
	{
		animInstance->Montage_Play(FireWeaponMontage);
		// FName  SectionName = bAim ? FName("FireAim") : FName("FireIronsight");
		FName SectionName = FName("Fire_Rifle_Ironsights");
		animInstance->Montage_JumpToSection(SectionName);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayFireMontage");
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Playing Fire Montage");
} 
