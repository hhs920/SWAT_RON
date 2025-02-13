// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/RONComponents/PlayerCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"

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

void UPlayerCombatComponent::SetUpInitialEquippedWeapon()
{
	Super::SetUpInitialEquippedWeapon();
}

void UPlayerCombatComponent::ChangeEquipment(EEquipmentType Type)
{
	Super::ChangeEquipment(Type);

	// 기존에 들고있던 무기를 EquipmentType에 맞는 소켓에 돌려놓는다(Attach).
	switch (EquippedWeapon->GetEquipmentType())
	{
	case EEquipmentType::EET_Grenade:
		{
			if (GrenadeSocket)
			{
				GrenadeSocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	case EEquipmentType::EET_Tactical:
		{
			if (TacticalSocket)
			{
				TacticalSocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	case EEquipmentType::EET_LongTactical:
		{
			if (LongTacticalSocket)
			{
				LongTacticalSocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	case EEquipmentType::EET_CableTie:
		{
			if (CableTieSocket)
			{
				CableTieSocket->AttachActor(EquippedWeapon, Character->GetMesh());
			}
		}
		break;
	}

	// 바꿔들을 무기를 RightHandSocket에 Attach한다.
	switch (Type)
	{
	case EEquipmentType::EET_Grenade:
		{
			if (GrenadeSocket)
			{
				GrenadeSocket->AttachActor(Grenade, Character->GetMesh());
				RightHandSocket->AttachActor(Grenade, Character->GetMesh());
				EquippedWeapon = Grenade;
			}
		}
		break;
	case EEquipmentType::EET_Tactical:
		{
			if (TacticalSocket)
			{
				TacticalSocket->AttachActor(Tactical, Character->GetMesh());
				RightHandSocket->AttachActor(Tactical, Character->GetMesh());
				EquippedWeapon = Tactical;
			}
		}
		break;
	case EEquipmentType::EET_LongTactical:
		{
			if (LongTacticalSocket)
			{
				LongTacticalSocket->AttachActor(LongTactical, Character->GetMesh());
				RightHandSocket->AttachActor(LongTactical, Character->GetMesh());
				EquippedWeapon = LongTactical;
			}
		}
		break;
	case EEquipmentType::EET_CableTie:
		{
			if (CableTieSocket)
			{
				CableTieSocket->AttachActor(CableTie, Character->GetMesh());
				RightHandSocket->AttachActor(CableTie, Character->GetMesh());
				EquippedWeapon = CableTie;
			}
		}
		break;
	}
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
	Tactical = GetWorld()->SpawnActor<AWeapon>(TacticalWeaponClass);
	LongTactical = GetWorld()->SpawnActor<AWeapon>(LongTacticalWeaponClass);
	CableTie = GetWorld()->SpawnActor<AWeapon>(CableTieWeaponClass);

	Grenade->SetWeaponState(EWeaponState::EWS_Equipped);
	Tactical->SetWeaponState(EWeaponState::EWS_Equipped);
	LongTactical->SetWeaponState(EWeaponState::EWS_Equipped);
	CableTie->SetWeaponState(EWeaponState::EWS_Equipped);

	Grenade->SetWeaponType(EEquipmentType::EET_Grenade);
	Tactical->SetWeaponType(EEquipmentType::EET_Tactical);
	LongTactical->SetWeaponType(EEquipmentType::EET_LongTactical);
	CableTie->SetWeaponType(EEquipmentType::EET_CableTie);
	
	GrenadeSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("GrenadeSocket"));
	if (GrenadeSocket)
		GrenadeSocket->AttachActor(Grenade, PlayerCharacter->GetMesh());

	TacticalSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("TacticalSocket"));
	if (TacticalSocket)
		TacticalSocket->AttachActor(Tactical, PlayerCharacter->GetMesh());

	LongTacticalSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("LongTacticalSocket"));
	if (LongTacticalSocket)
		LongTacticalSocket->AttachActor(LongTactical, PlayerCharacter->GetMesh());

	CableTieSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("CableTieSocket"));
	if (CableTieSocket)
		CableTieSocket->AttachActor(CableTie, PlayerCharacter->GetMesh());
}

void UPlayerCombatComponent::Fire()
{
	Super::Fire();
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
			DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV,
			DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
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
