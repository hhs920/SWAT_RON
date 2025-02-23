// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/Casing.h"

#include "ReadyOrNot.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = true;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true); // = Simulation Generates Hit Events 설정이다.
	
	ShellEjectionImpulse = 7.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	ShellEjectionImpulse += FMath::RandRange(-3, 3);
	CasingMesh->AddImpulse(GetActorRightVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, ShellDropSound, GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(ShellDestroyTimer,
		[this](){this->Destroy();},
		5.f, false);
}

