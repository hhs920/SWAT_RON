// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/Weapon.h"

#include "CSW/Weapon/Casing.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// GatherEvidenceWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GatherEvidenceWidget"));
	// GatherEvidenceWidget->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// if (GatherEvidenceWidget)
	// {
	// 	ShowGatherEvidenceWidget(false);
	// }

	if (bUseSemiAuto) AvailableSelectorStates.Add(ESelectorState::SemiAuto);
	if (bUseBurst) AvailableSelectorStates.Add(ESelectorState::Burst);
	if (bUseFullAuto) AvailableSelectorStates.Add(ESelectorState::FullAuto);
}



void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWeapon::OnBeginEquip()
{
}

void AWeapon::OnEndEquip()
{
	SetEquippedState(EEquippedState::Equipped);

	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bUsing = false;
	
	// 소유자 세팅
	// OwnerCharacter =  
}

void AWeapon::OnBeginUnequip()
{
	bUsing = false;
}

void AWeapon::OnEndUnequip()
{
}

void AWeapon::OnBeginUse()
{
	bUsing = true;
	Fire(LineTraceTarget);
}

void AWeapon::OnEndUse()
{
	bUsing = false;
}

void AWeapon::OnBeginInteract()
{
}

void AWeapon::OnEndInteract()
{
}



void AWeapon::Fire(FVector& HitTarget)
{
	if (FireAnim)
	{
		MeshComp->PlayAnimation(FireAnim, false);		
	}

	// 점사면 점사 횟수 카운트
	if (SelectorState == ESelectorState::Burst)
	{
		BurstFireCount++;
		if (BurstFireCount >= MaxBurstCount)
		{
			GetWorld()->GetTimerManager().ClearTimer(FireTimer);
			BurstFireCount = 0;
		}
	}

	if (CasingClass)
	{
		if (AmmoEjectSocket == nullptr)
			AmmoEjectSocket = GetMesh()->GetSocketByName(FName("AmmoEject"));
	
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetMesh());
			FActorSpawnParameters SpawnParams;
			GetWorld()->SpawnActor<ACasing>(CasingClass,
				SocketTransform.GetLocation(),
				SocketTransform.GetRotation().Rotator(),
				SpawnParams);
		}
	}
}



void AWeapon::Reload()
{
	if (ReloadAnim)
	{
		MeshComp->PlayAnimation(ReloadAnim, false);		
	}
}



void AWeapon::OnDropped()
{
	SetEquippedState(EEquippedState::Dropped);
	// 소켓에 붙은 것을 뗀다

	// 중력을 적용한다.
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



// void AWeapon::ShowGatherEvidenceWidget(bool bShowWidget)
// {
// 	//GatherEvidenceWidget->SetVisibility(bShowWidget);
// }

FVector AWeapon::TraceEndWithScatter(const FVector& HitTarget)
{
	return FVector();
}

bool AWeapon::IsEmpty()
{
	return Ammo <= 0;
}

void AWeapon::SetBurstFireCount(int32 cnt)
{
	BurstFireCount = cnt;
}

void AWeapon::ChangeSelectorState()
{
	int32 num = AvailableSelectorStates.Num();
	int32 curIdx = AvailableSelectorStates.Find(SelectorState);
	int32 nextIdx = curIdx + 1;

	// 없거나 다음이 크기를 넘어갔으면
	if (num == 0 || curIdx == INDEX_NONE || nextIdx >= AvailableSelectorStates.Num())
	{
		SelectorState = ESelectorState::SemiAuto;
		return;
	}
	
	SelectorState = AvailableSelectorStates[nextIdx];
}
