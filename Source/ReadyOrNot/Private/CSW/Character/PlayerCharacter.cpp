// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "CSW/Character/PlayerInputComponent.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


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
	// SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	// SpringArmComp->SetupAttachment(GetMesh());
	// SpringArmComp->TargetArmLength = 0.f; // FPS
	// SpringArmComp->bUsePawnControlRotation = true; // 마우스 입력에 따라 회전
	// SpringArmComp->SetRelativeLocation(FVector(0.f, 10.f, 170.f));
	
	// CameraComp 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetMesh());

	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	// CharacterMovement의 Crouch 기능 켜기
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	SetupStimulusSource();	// AI 인식
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

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComp)
	{
		CombatComp->PlayerCharacter = this;
	}

	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("Camera"));
	CameraComp->bUsePawnControlRotation = true;
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

void APlayerCharacter::PrimaryEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PrimaryEquip");
}

void APlayerCharacter::SecondaryEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SecondaryEquip");
}

void APlayerCharacter::GrenadeEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "GrenadeEquip");
}

void APlayerCharacter::TacticalEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TacticalEquip");
}

void APlayerCharacter::LongTacticalEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LongTacticalEquip");
}

void APlayerCharacter::CableTieEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CableTieEquip");
}

void APlayerCharacter::LeanLeft(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LeanLeft");
}

void APlayerCharacter::LeanRight(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LeanRight");
}

void APlayerCharacter::LowReady(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LowReady");
}

void APlayerCharacter::CrouchStarted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchStarted");

	if (!bIsCrouched)
	{
		PlayerStance = EPlayerStance::EPS_Crouching;
		Crouch();
	}
}

void APlayerCharacter::CrouchCompleted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchCompleted");
	if (bIsCrouched)
	{
		PlayerStance = EPlayerStance::EPS_Aiming;
		UnCrouch();
	}
}

void APlayerCharacter::Reload(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Reload");
}

void APlayerCharacter::ChangeSelector(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ChangeSelector");

}

void APlayerCharacter::Interact(const FInputActionValue& inputValue)
{
	// TODO : LineTrace를 쏴서 Interact 대상이 총기(증거품)인지 범죄증거물인지 확인해야한다.
	
	if (CombatComp)
	{
		CombatComp->EquipWeapon(InteractingWeapon);
	}
}

void APlayerCharacter::AimStarted(const FInputActionValue& inputValue)
{
	if (CombatComp)
	{
		CombatComp->bAiming = true;
	}
}

void APlayerCharacter::AimCompleted(const FInputActionValue& inputValue)
{
	if (CombatComp)
	{
		CombatComp->bAiming = false;
	}
}

void APlayerCharacter::SetInteractingWeapon(AWeapon* Weapon)
{
	if (InteractingWeapon)
	{
		InteractingWeapon->ShowGatherEvidenceWidget(false);
	}

	InteractingWeapon = Weapon;
	if (InteractingWeapon)
	{
		InteractingWeapon->ShowGatherEvidenceWidget(true);
	}
}

EEquipmentType APlayerCharacter::GetEquipmentType()
{
	if (CombatComp == nullptr)
		return EEquipmentType::None;
	return CombatComp->EquipmentType;
}

bool APlayerCharacter::IsAiming()
{
	return CombatComp && CombatComp->bAiming;
}

void APlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}


