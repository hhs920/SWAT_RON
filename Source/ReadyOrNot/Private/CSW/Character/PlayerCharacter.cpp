// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "CSW/RONComponents/PlayerCombatComponent.h"
#include "CSW/Character/PlayerInputComponent.h"
#include "CSW/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	CombatComp = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComp"));

	// CharacterMovement의 Crouch 기능 켜기
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
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
		CombatComp->Character = this;
		CombatComp->PlayerCharacter = this;
	}

	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("Camera"));
	CameraComp->bUsePawnControlRotation = true;
}

void APlayerCharacter::AimOffset(float DeltaTime)
{
	if (CombatComp && CombatComp->EquippedWeapon == nullptr) return;
	
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	// AO_Yaw 세팅
	// if (Speed == 0.f) // 멈춰있다
	// {
	// 	FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
	// 	FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
	// 	AO_Yaw = DeltaAimRotation.Yaw;
	// 	bUseControllerRotationPitch = false;
	// }
	// if (Speed > 0.f) // 움직이고 있다.
	// {
	// 	StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
	// 	AO_Yaw = 0.f;
	// 	bUseControllerRotationPitch = true;
	// }

	// AO_Pitch 세팅
	AO_Pitch = GetBaseAimRotation().Pitch;
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

void APlayerCharacter::FireStarted(const FInputActionValue& inputValue)
{
	if (CombatComp)
	{
		CombatComp->FireButtonPressed(true);
	}
}

void APlayerCharacter::FireCompleted(const FInputActionValue& inputValue)
{
	if (CombatComp)
	{
		CombatComp->FireButtonPressed(false);
	}
}

void APlayerCharacter::PrimaryEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PrimaryEquip");
	ChangeEquipment(EEquipmentType::EET_Primary);
}

void APlayerCharacter::SecondaryEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SecondaryEquip");
	ChangeEquipment(EEquipmentType::EET_Secondary);
}

void APlayerCharacter::GrenadeEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "GrenadeEquip");
	ChangeEquipment(EEquipmentType::EET_Grenade);
}

void APlayerCharacter::TacticalEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TacticalEquip");
	ChangeEquipment(EEquipmentType::EET_Tactical);
}

void APlayerCharacter::LongTacticalEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LongTacticalEquip");
	ChangeEquipment(EEquipmentType::EET_LongTactical);
}

void APlayerCharacter::CableTieEquip(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CableTieEquip");
	ChangeEquipment(EEquipmentType::EET_CableTie);
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

	if (!bIsCrouched)
	{
		if (_stance == EPlayerStance::EPS_Assault)
		{
			_stance = EPlayerStance::EPS_LowReady;
			GetCharacterMovement()->MaxWalkSpeed = LowReadyWalkSpeed;
		}
		else if (_stance == EPlayerStance::EPS_LowReady)
		{
			_stance = EPlayerStance::EPS_Assault;
			GetCharacterMovement()->MaxWalkSpeed = AssaultWalkSpeed;
		}
	}
}

void APlayerCharacter::CrouchStarted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchStarted");

	if (!bIsCrouched)
	{
		_stance = EPlayerStance::EPS_Crouching;

		Crouch();
	}
}

void APlayerCharacter::CrouchCompleted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchCompleted");
	if (bIsCrouched)
	{
		_stance = EPlayerStance::EPS_Assault;
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
		//CombatComp->EquipWeapon(InteractingWeapon);
	}
}

void APlayerCharacter::AimStarted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AimStarted");

	if (CombatComp)
	{
		if (CombatComp->EquippedWeapon->GetCanZoom())
		{
			CombatComp->SetAiming(true);
			//_stance = EPlayerStance::EPS_Assault;
			
			GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchAimWalkSpeed;
		}
	}
}

void APlayerCharacter::AimCompleted(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AimCompleted");

	if (CombatComp)
	{
		if (CombatComp->EquippedWeapon->GetCanZoom())
		{
			CombatComp->SetAiming(false);

			if (_stance == EPlayerStance::EPS_Assault)
			{
				GetCharacterMovement()->MaxWalkSpeed = AssaultWalkSpeed;
			}
			else if (_stance == EPlayerStance::EPS_LowReady)
			{
				GetCharacterMovement()->MaxWalkSpeed = LowReadyWalkSpeed;
			}
			else if (_stance == EPlayerStance::EPS_Crouching)
			{
				GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchWalkSpeed;
			}

		}
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

void APlayerCharacter::PlayFireMontage(bool bAiming)
{
	if (CombatComp == nullptr || CombatComp->EquippedWeapon == nullptr)
		return;
	
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	// Aim(줌인) 상태와 Ironsights(Assault 스탠스)일 때 다른 애니메이션을 출력한다.
	if (animInstance && FireWeaponMontage)
	{
		animInstance->Montage_Play(FireWeaponMontage);
		// FName  SectionName = bAiming ? FName("FireAim") : FName("FireIronsight");
		FName SectionName = FName("Fire_Rifle_Ironsights");
		animInstance->Montage_JumpToSection(SectionName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayFireMontage");
	}
} 

EEquipmentType APlayerCharacter::GetEquipmentType()
{
	if (CombatComp == nullptr || CombatComp->EquippedWeapon == nullptr)
		return EEquipmentType::EET_None;

	return CombatComp->EquippedWeapon->GetEquipmentType();
}

// 플레이어의 입력을 받아서 weapon을 Set한다.
void APlayerCharacter::ChangeEquipment(EEquipmentType Type)
{
	if (CombatComp == nullptr)
		return;

	// 이미 들고있는 무기면 return
	if (GetEquippedWeapon()->GetEquipmentType() == Type)
		return;
	
	CombatComp->ChangeEquipment(Type);
}

bool APlayerCharacter::IsAiming()
{
	return CombatComp && CombatComp->GetAiming();
}

AWeapon* APlayerCharacter::GetEquippedWeapon() const
{
	if (CombatComp == nullptr) return nullptr;

	return CombatComp->EquippedWeapon;
}


