// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/PistolEnemy.h"

#include "PistolEnemyFSM.h"
#include "CSW/RONComponents/CombatComponent.h"


APistolEnemy::APistolEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/asset/PistolEnemy/PistolEnemy.PistolEnemy'"));
	if (MeshTmp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	
	//GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	//GunMeshComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
   	
	//// 스켈레탈 메시 데이터 로드
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/asset/PistolEnemy/Pistols_B.Pistols_B'"));
	//// 데이터 로드가 성공했다면
	//if (TempGunMesh.Succeeded())
	//{
	//	// 스켈레탈 메시 데이터 할당
	//	GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
   //
	//	// 총 위치 설정
	//	GunMeshComp->SetRelativeLocationAndRotation(FVector(-1.926740, -9.613768, 4.732659) , FRotator(74.206776, -378.676810, -558.018135) );
	//}
	
	FSM = CreateDefaultSubobject<UPistolEnemyFSM>(TEXT("FSM"));
	
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/HHS/BluePrints/PistolEnemy/ABP_PistolEnemy.ABP_PistolEnemy'"));

	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
	
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

// Called when the game starts or when spawned
void APistolEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APistolEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (CombatComp)
	{
		CombatComp->Character=this;
		CombatComp->SetUpEquipments();
	}
}


// Called every frame
void APistolEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APistolEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

