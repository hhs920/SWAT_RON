// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/Weapon.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// GatherEvidenceWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GatherEvidenceWidget"));
	// GatherEvidenceWidget->SetupAttachment(RootComponent);
	
	bCanZoom = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// if (GatherEvidenceWidget)
	// {
	// 	ShowGatherEvidenceWidget(false);
	// }
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnBeginUse()
{
	Super::OnBeginUse();

	// HitTarget을 계산한다.
	// Fire();
}

void AWeapon::Fire(FVector& HitTarget)
{
	if (FireAnim)
	{
		MeshComp->PlayAnimation(FireAnim, false);		
	}
}

void AWeapon::Reload()
{
	if (ReloadAnim)
	{
		MeshComp->PlayAnimation(ReloadAnim, false);		
	}
}

void AWeapon::OnEquipped()
{
	SetEquippedState(EEquippedState::Equipped);

	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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

void AWeapon::SetSelectorState(ESelectorState State)
{
	SelectorState = State;
}
