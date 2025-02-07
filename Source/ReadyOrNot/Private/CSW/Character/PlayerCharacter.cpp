// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerInputComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메시 세팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/CSW/SWAT/SK_SWAT.SK_SWAT'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f),
			FRotator(0.f, -90.f, 0.f));
	}

	// 인풋 컴포넌트 만들어 넣기
	InputComp = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComp"));

	// 스프링암과 카메라	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->TargetArmLength = 0.f; // FPS
	SpringArmComp->bUsePawnControlRotation = true; // 마우스 입력에 따라 회전
	SpringArmComp->SetRelativeLocation(FVector(0.f, 10.f, 170.f));
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	 // SpringArmComponent.cpp - const FName USpringArmComponent::SocketName(TEXT("SpringEndpoint"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComp->SetUpInputMappingContext(Controller);
	InputComp->SetUpPlayerInputAction(PlayerInputComponent);
}

void APlayerCharacter::PlayerMove(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>().GetSafeNormal();
	MoveDir.X = value.X;
	MoveDir.Y = value.Y;
	
	FVector localMoveDir = FTransform(GetControlRotation()).TransformVector(MoveDir);
	AddMovementInput(localMoveDir);
	
	MoveDir = FVector::ZeroVector;

    UE_LOG(LogTemp, Warning, TEXT("PlayerMove"));
}

void APlayerCharacter::PlayerTurn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value * TurnSpeed * GetWorld()->GetDeltaSeconds());
	UE_LOG(LogTemp, Warning, TEXT("PlayerTurn"));

}

void APlayerCharacter::PlayerLookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value * LookUpSpeed * GetWorld()->GetDeltaSeconds()); // Picth는 x축기준 회전
	UE_LOG(LogTemp, Warning, TEXT("PlayerLookUp"));

}



