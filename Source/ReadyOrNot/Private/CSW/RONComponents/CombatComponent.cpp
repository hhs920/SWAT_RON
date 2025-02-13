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

	// 주무기를 처음에 들고있는다.
	HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Primary, Character->GetMesh());
		EquippedWeapon = Primary;

	}
	
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
	Secondary = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
	Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);
	
	Primary->SetWeaponType(EEquipmentType::EET_Primary);
	Secondary->SetWeaponType(EEquipmentType::EET_Secondary);
	Grenade->SetWeaponType(EEquipmentType::EET_Grenade);
	Tactical->SetWeaponType(EEquipmentType::EET_Tactical);
	LongTactical->SetWeaponType(EEquipmentType::EET_LongTactical);
	CableTie->SetWeaponType(EEquipmentType::EET_CableTie);

	// 플레이어의 소켓에 부착한다.
	PrimarySocket = Character->GetMesh()->GetSocketByName(FName("PrimarySocket"));
	if (PrimarySocket)
		PrimarySocket->AttachActor(Primary, Character->GetMesh());

	SecondarySocket = Character->GetMesh()->GetSocketByName(FName("SecondarySocket"));
	if (SecondarySocket)
		SecondarySocket->AttachActor(Secondary, Character->GetMesh());

	GrenadeSocket = Character->GetMesh()->GetSocketByName(FName("GrenadeSocket"));
	if (GrenadeSocket)
		GrenadeSocket->AttachActor(Grenade, Character->GetMesh());

	TacticalSocket = Character->GetMesh()->GetSocketByName(FName("TacticalSocket"));
	if (TacticalSocket)
		TacticalSocket->AttachActor(Tactical, Character->GetMesh());

	LongTacticalSocket = Character->GetMesh()->GetSocketByName(FName("LongTacticalSocket"));
	if (LongTacticalSocket)
		LongTacticalSocket->AttachActor(LongTactical, Character->GetMesh());

	CableTieSocket = Character->GetMesh()->GetSocketByName(FName("CableTieSocket"));
	if (CableTieSocket)
		CableTieSocket->AttachActor(CableTie, Character->GetMesh());

}

void UCombatComponent::ChangeEquipment(EEquipmentType EquipmentType)
{
}

void UCombatComponent::AttachEquipmentToSocket(EEquipmentType EquipmentType)
{
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
		if (bFireButtonPressed)
		{
			Character->PlayFireMontage(bAiming);
			
		}
		else
		{
			// 총기가 연사모드인지 단발모드인지 점사모드인지 알아야한다.
		}
	}
}

void UCombatComponent::PickUpWeapon(class AWeapon* WeaponToEquip)
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

	HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
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




