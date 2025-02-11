// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayer.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATestPlayer::ATestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SK)_Mannequin 파일을 열고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/Anim/SK_Mannequin.SK_Mannequin'"));

	// 만약 파일 읽기가 성공했다면
	if (MeshTmp.Succeeded())
	{
		// 로드한 메시를 넣어주고 싶다.
		GetMesh()->SetSkeletalMesh(MeshTmp.Object);
		// 위치값과 회전값을 반영해주고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
	}

	// TPS 카메라를 붙이고 싶다.
	// SpringArm 컴포넌트 붙이기
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment( RootComponent );
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f ) );
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// camera 컴포넌트 붙이기
	TpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsComp"));
	TpsCamComp->SetupAttachment(SpringArmComp);
	TpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	// 총 스켈레탈 메시 컴포넌트 등록
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	GunMeshComp->SetupAttachment(GetMesh());
	
	// 스켈레탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/Anim/SK_KA_Val_X.SK_KA_Val_X'"));

	// 데이터 로드가 성공했다면
	if (TempGunMesh.Succeeded())
	{
		// 스켈레탈 메시 데이터 할당
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		// 총 위치 설정
		GunMeshComp->SetRelativeLocation(FVector(0.0f, 60.0f, 120.0f) );
	}
}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//if (PlayerInput)
	//{
	//	PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ATestPlayer::Turn);
	//	PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ATestPlayer::LookUp);
	//	PlayerInput->BindAction(IA_MOVE, ETriggerEvent::Triggered, this, &ATestPlayer::Move);
//
	//	//// 총알 발사 이벤트 처리 함수 바인딩
	//	//PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATestPlayer::InputFire);
	//}
}

//void ATestPlayer::Turn(const FInputActionValue& inputValue)
//{
//	float value = inputValue.Get<float>();
//	AddControllerYawInput(value);
//}
//
//void ATestPlayer::LookUp(const FInputActionValue& inputValue)
//{
//	float value = inputValue.Get<float>();
//	AddControllerPitchInput(value);
//}
//
//void ATestPlayer::Move(const FInputActionValue& inputValue)
//{
//	FVector2D value = inputValue.Get<FVector2D>();
//	
//	Direction.X = value.X;
//
//	Direction.Y = value.Y;
//}

//void ATestPlayer::InputFire(const struct FInputActionValue& InputValue)
//{
//}


