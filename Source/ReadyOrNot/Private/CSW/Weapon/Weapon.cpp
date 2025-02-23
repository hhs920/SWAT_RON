// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/Weapon.h"

#include "Components/WidgetComponent.h"
#include "CSW/Weapon/Casing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HHS/HHS_GameMode.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (GatherEvidenceWidget) // UI가 이미 생성되지 않았다면
	{
		// 위젯을 생성하고 변수에 저장
		GatherEvidenceUI = CreateWidget<UUserWidget>(GetWorld(), GatherEvidenceWidget);
        
		if (GatherEvidenceUI)
		{
			GatherEvidenceUI->AddToViewport(); // 화면에 추가
			ShowGatherEvidenceWidget(false);
		}
	}

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
	// 무기 증거 수집
	if (EquippedState != EEquippedState::Dropped) return;
	
	// 박스를 생성한다.
	GetWorld()->SpawnActor<AActor>(BoxGathered, GetActorTransform());

	// 무기를 없앤다.
	if (GatherEvidenceUI)
	{
		GatherEvidenceUI->RemoveFromParent(); // UI 제거
		GatherEvidenceUI = nullptr;
	}
	
	if (AHHS_GameMode* GM = Cast<AHHS_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		// 증거 수집 개수 +1
		GM->IncreaseWeaponCount();
	}
	
	Destroy();	
}

void AWeapon::OnEndInteract()
{
	if (EquippedState != EEquippedState::Dropped) return;
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

void AWeapon::ShowGatherEvidenceWidget(bool bShowWidget)
{
	if (bShowWidget)
	{
		if (GatherEvidenceUI)
			GatherEvidenceUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (GatherEvidenceUI)
			GatherEvidenceUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

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
	if (bUsing)
		return;
	
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

void AWeapon::SetSelectorState(ESelectorState state)
{
	SelectorState = state;

}

