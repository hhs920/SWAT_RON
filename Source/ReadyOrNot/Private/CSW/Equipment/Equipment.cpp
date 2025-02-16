// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Equipment/Equipment.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AEquipment::AEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	MeshComp->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	// Pawn은 무기를 밟고 지나갈 수 있다.
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	
}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipment::SetEquippedState(EEquippedState State)
{
	EquippedState = State;
}

void AEquipment::SetEquipmentType(EEquipmentType Type)
{
	EquipmentType = Type;
}

class USkeletalMeshSocket const* AEquipment::GetSocketToEquipped() const
{
	switch (EquipmentType)
	{
	case EEquipmentType::Primary:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("PrimarySocket"));
	case EEquipmentType::Secondary:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("SecondarySocket"));
	case EEquipmentType::Grenade:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("GrenadeSocket"));
	case EEquipmentType::Tactical:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("TacticalSocket"));
	case EEquipmentType::LongTactical:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("LongTacticalSocket"));
	case EEquipmentType::CableTie:
		return OwnerCharacter->GetMesh()->GetSocketByName(FName("CableTieSocket"));
	}
	
	return nullptr;
}

void AEquipment::Equip()
{
	OnBeginEquip();
	OnEndEquip();
}

void AEquipment::Unequip()
{
	OnBeginUnequip();
	OnEndUnequip();
}

void AEquipment::Use()
{
	OnBeginUse();
	OnEndUse();
}

void AEquipment::Interact()
{
	OnBeginInteract();
	OnEndInteract();
}



