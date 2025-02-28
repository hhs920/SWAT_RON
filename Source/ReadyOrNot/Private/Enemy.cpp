// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include "CSW/RONComponents/CombatComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/asset/Enemy/Ch18_nonPBR.Ch18_nonPBR'"));
	if (MeshTmp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
/*
	// 총 스켈레탈 메시 컴포넌트 등록
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
   	
   	// 부모 컴포넌트를 Mesh 컴포넌트로 설정	
   	GunMeshComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
   	
   	// 스켈레탈 메시 데이터 로드
   	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Common/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SK_KA_Val_X.SK_KA_Val_X'"));
   
   	// 데이터 로드가 성공했다면
   	if (TempGunMesh.Succeeded())
   	{
   		// 스켈레탈 메시 데이터 할당
   		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
   
   		// 총 위치 설정
   		GunMeshComp->SetRelativeLocationAndRotation(FVector(-2.126416, -8.522827, 2.809152) , FRotator(14.477511, -74.495910, 93.967131) );
   	}
   	*/
	
	// EnemyFSM 컴포넌트 추가
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 애니메이션 블루프린트 할당
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/HHS/BluePrints/ABP_Enemy.ABP_Enemy'"));

	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// 컴포넌트들이 생성되고나서 BeginPlay() 전에 호출된다.
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (CombatComp && CombatComp->Character)
	{
		CombatComp->Character=this;
	}
	CombatComp->SetUpEquipments();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

