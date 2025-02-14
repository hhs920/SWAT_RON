// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/RunEnemy.h"

#include "HHS/RunEnemyFSM.h"

// Sets default values
ARunEnemy::ARunEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (MeshTmp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	FSM = CreateDefaultSubobject<URunEnemyFSM>(TEXT("FSM"));
	
	//// 애니메이션 블루프린트 할당
	//ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/HHS/BluePrints/ABP_Enemy.ABP_Enemy_C'"));
//
	//if (tempClass.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(tempClass.Class);
	//}
}

// Called when the game starts or when spawned
void ARunEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARunEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

