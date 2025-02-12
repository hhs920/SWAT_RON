// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/PlayerCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
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

void UPlayerCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayFireMontage(bAiming);
	}
}

void UPlayerCombatComponent::SetUpEquipments()
{
	Super::SetUpEquipments();

	Grenade = GetWorld()->SpawnActor<AWeapon>(GrenadeWeaponClass);
	Grenade->SetWeaponType(EEquipmentType::EET_Grenade);

	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	Tactical->SetWeaponType(EEquipmentType::EET_Tactical);
	
	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	LongTactical->SetWeaponType(EEquipmentType::EET_LongTactical);

	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);
	CableTie->SetWeaponType(EEquipmentType::EET_CableTie);
	
	PlayerCharacter->SetEquippedWeapon(Primary);

	// TODO : 플레이어 캐릭터의 장비로 만들기 // 장비X일 때의 위치에 있도록 하기
}

void UPlayerCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	if (PlayerCharacter == nullptr || WeaponToEquip == nullptr)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "EquipWeapon");

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
	}}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		InterpFOV(DeltaTime);
	}
}

void UPlayerCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (bAiming)
	{
		// TODO : 무기에 달린 카메라로 Set View Target with Blend 처리
		
	}
	else
	{
		// TODO : 플레이어에 달린 카메라로 Set View Target with Blend 처리
		
	}
}

void UPlayerCombatComponent::InterpFOV(float DeltaTime)
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

	if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV); 
	}
}

void UPlayerCombatComponent::GatherEvidence(class AWeapon* EvidenceToGather)
{
	if (Character == nullptr || EvidenceToGather == nullptr)
		return;
	
	EvidenceToGather->SetWeaponState(EWeaponState::EWS_Gathered);
}




