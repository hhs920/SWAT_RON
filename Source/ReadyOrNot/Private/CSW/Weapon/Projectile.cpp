// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/Projectile.h"

#include "ReadyOrNot.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox"));
	SetRootComponent(CollisionBox);
		// World에서 날라가므로 WorldDynamic
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		// 보이는 것에는 물리충돌(Block) 되도록한다.
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh,  ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true; // 속도에 맞춰 회전 // 중력에 따라 경로가 바뀌는데 영향을 준다.
	ProjectileMovement->InitialSpeed = 15000.f; // 150 m/s
	ProjectileMovement->MaxSpeed = 15000.f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionBox);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer, CollisionBox, FName(),
			GetActorLocation(), GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
	
	// Hit 충돌 이벤트 바인딩
	CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector Normal, const FHitResult& HitResult)
{
	
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

