// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerCharacter.h"

#include "Enemy.h"
#include "EnemyFSM.h"
#include "EngineUtils.h"
#include "InputActionValue.h"
#include "PistolEnemyFSM.h"
#include "ReadyOrNot.h"
#include "Camera/CameraComponent.h"
#include "CSW/RONComponents/PlayerCombatComponent.h"
#include "CSW/Character/PlayerInputComponent.h"
#include "CSW/Character/RONPlayerController.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "CSW/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"	
#include "GameFramework/SpringArmComponent.h"
#include "HHS/PistolEnemy.h"
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
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f),
			FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	

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

	SetupStimulusSource();	// AI 인식

	// ADS
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Script/Engine.CurveFloat'/Game/CSW/Miscellaneous/CV_ADS.CV_ADS'"));
	if (curve.Succeeded())
	{
		AdsCurve = curve.Object;
	}
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	RONPlayerController = Cast<ARONPlayerController>(GetController());
	if (RONPlayerController)
	{
		UpdateHealthHUD();
		OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::OnReceiveDamage);
	}

  
	//AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	FOnTimelineFloat AdsUpdate; // 델리게이트 생성
	AdsUpdate.BindUFunction(this, FName("OnAdsUpdate")); // 바인딩

	// AdsCurve데이터의 값이 변경될 때 AdsUpdate가 호출된다.
	// 즉, AdsTimeline.Play() 또는 .Reverse() 시에 호출된다.
	AdsTimeline.AddInterpFloat(AdsCurve, AdsUpdate);

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);

	AdsTimeline.TickTimeline(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComp->SetUpInputMappingContext(Controller);
	InputComp->SetUpPlayerInputAction(PlayerInputComponent);

	// // #$!@$##!$#$!@  <<<Enemy 데미지 테스트용>>>  @#$!#!@$!$@#@$#!$!@!$$!$
	//PlayerInputComponent->BindAction("DamageEnemy", IE_Pressed, this, &APlayerCharacter::DamageToEnemy);
	PlayerInputComponent->BindAction("DamagePistolEnemy", IE_Pressed, this, &APlayerCharacter::DamageToPistolEnemy);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComp)
	{
		CombatComp->Character = this;
		CombatComp->PlayerCharacter = this;
		CombatComp->SetUpEquipments();
	}

	CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("Camera"));
	CameraComp->bUsePawnControlRotation = true;
}

void APlayerCharacter::UpdateHealthHUD()
{
	RONPlayerController = Cast<ARONPlayerController>(GetController());
	if (RONPlayerController)
	{
		RONPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void APlayerCharacter::AimOffset(float DeltaTime)
{
	if (CombatComp && CombatComp->HoldingEquipment == nullptr) return;
	
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

// Alpha는 0.0 ~ 1.0 사이의 값이다. Timeline에서 현재 진행 상태이다.
void APlayerCharacter::OnAdsUpdate(float Alpha)
{
	// 카메라 트랜스폼
	FTransform camSocket = GetMesh()->GetSocketTransform(FName("Camera"));
	
	// 손에 들고있는 장비의 소켓
	FTransform adsSocket = GetHoldingEquipment()->GetMesh()->GetSocketTransform(FName("Ads"));
		// Ads소켓보다 20cm 뒤의 위치
	adsSocket.SetLocation(adsSocket.GetLocation() + adsSocket.GetRotation().GetForwardVector() * (-1) * GetHoldingEquipment()->GetDistanceFromADS());

	// 위치 보간 (FVector 사용)
	FVector lerpedLocation = FMath::Lerp(camSocket.GetLocation(), adsSocket.GetLocation(), Alpha);

	// 회전 보간 (FRotator 사용)
	FRotator lerpedRotation = FMath::Lerp(camSocket.GetRotation().Rotator(), adsSocket.GetRotation().Rotator(), Alpha);

	// 보간된 값으로 새로운 Transform 생성
	FTransform tr;
	tr.SetLocation(lerpedLocation);
	tr.SetRotation(lerpedRotation.Quaternion());

	// 카메라 Transform 적용
	GetFollowCamera()->SetWorldTransform(tr);
}

void APlayerCharacter::OnReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
   class AController* InstigatorController, AActor* DamageCauser)
{
	float currentTime = GetWorld()->GetUnpausedTimeSeconds();
	if (LastDamagedTime + DamagedDelayTime < currentTime)
	{
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
		UpdateHealthHUD();
		LastDamagedTime = currentTime;
	}
}

void APlayerCharacter::PlayerMove(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>().GetSafeNormal();
	MoveDir.X = value.X;
	MoveDir.Y = value.Y;
	
	FVector localMoveDir = FTransform(GetControlRotation()).TransformVector(MoveDir);
	AddMovementInput(localMoveDir);
	
	MoveDir = FVector::ZeroVector;

    //UE_LOG(LogTemp, Warning, TEXT("PlayerMove"));
}

void APlayerCharacter::PlayerTurn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value * TurnSpeed * GetWorld()->GetDeltaSeconds());
	//UE_LOG(LogTemp, Warning, TEXT("PlayerTurn"));

}

void APlayerCharacter::PlayerLookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value * LookUpSpeed * GetWorld()->GetDeltaSeconds()); // Picth는 x축기준 회전
	//UE_LOG(LogTemp, Warning, TEXT("PlayerLookUp"));

}

void APlayerCharacter::FireStarted(const FInputActionValue& inputValue)
{
	if (CombatComp)
	{
		CombatComp->FireButtonPressed(true);
		
		// LowReady 상태에서 Fire시 Assault로 바로 변경
		if (_stance == EPlayerStance::EPS_LowReady)
		{
			_stance = EPlayerStance::EPS_Assault;
		}
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
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PrimaryEquip");
	ChangeEquipment(CombatComp->Primary);
}

void APlayerCharacter::SecondaryEquip(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SecondaryEquip");
	ChangeEquipment(CombatComp->Secondary);
}

void APlayerCharacter::GrenadeEquip(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "GrenadeEquip");
	ChangeEquipment(CombatComp->Grenade);
}

void APlayerCharacter::TacticalEquip(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TacticalEquip");
	ChangeEquipment(CombatComp->Tactical);
}

void APlayerCharacter::LongTacticalEquip(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LongTacticalEquip");
	ChangeEquipment(CombatComp->LongTactical);
}

void APlayerCharacter::CableTieEquip(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CableTieEquip");
	ChangeEquipment(CombatComp->CableTie);
}

void APlayerCharacter::LeanStarted(const FInputActionValue& inputValue)
{
	if (bLeaning)
	{
		// 기울인 상태에서 QE 입력 시, LeaningResetProcess를 취소한다.
		GetWorld()->GetTimerManager().ClearTimer(LeanCompletedTimer);
	}

	bLeaning = true;
}

void APlayerCharacter::Lean(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>(); // -1 ~ 1

	// [-1, 1] 을 [-MaxRollValue, MaxRollValue]로 변환
	float clampedValue = FMath::GetMappedRangeValueClamped(FVector2d(-1, 1), FVector2d(-MaxLeanRollValue, MaxLeanRollValue), value);
	// 
	LeanRollValue = FMath::FInterpTo(LeanRollValue, clampedValue, GetWorld()->GetDeltaSeconds(), LeanInterpSpeed);
	// Bone 회전 값 넣어주기
}

void APlayerCharacter::LeanCompleted(const FInputActionValue& inputValue)
{
	// RollValue를 0으로 돌려놓기
	// Timer이용
	GetWorld()->GetTimerManager().SetTimer(LeanCompletedTimer, this, &ThisClass::LeaningResetProcess, GetWorld()->GetDeltaSeconds(), true);
}

void APlayerCharacter::LeaningResetProcess()
{
	// RollValue가 0이 될 때까지 RollValue FInterp 처리
	if (LeanRollValue != 0)
	{
		LeanRollValue = FMath::FInterpTo(LeanRollValue, 0, GetWorld()->GetDeltaSeconds(), LeanInterpSpeed);
	}
	else
	{
		// RollValue가 0이 되어, 기울이기 끝
		bLeaning = false; 
		GetWorld()->GetTimerManager().ClearTimer(LeanCompletedTimer); // 타이머 끝내기
	}
}

void APlayerCharacter::LowReady(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LowReady");

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
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchStarted");

	if (!bIsCrouched)
	{
		_stance = EPlayerStance::EPS_Crouching;

		Crouch();
	}
}

void APlayerCharacter::CrouchCompleted(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CrouchCompleted");
	if (bIsCrouched)
	{
		_stance = EPlayerStance::EPS_Assault;
		UnCrouch();
	}
}

void APlayerCharacter::Reload(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Reload");
}

void APlayerCharacter::ChangeSelector(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ChangeSelector");
	AWeapon* weapon = Cast<AWeapon>(GetHoldingEquipment());
	if (weapon)
	{
		weapon->ChangeSelectorState();
	}
}

void APlayerCharacter::Interact(const FInputActionValue& inputValue)
{
	if (CombatComp && CombatComp->interacting)
	{
		CombatComp->interacting->BeginInteract();
	}
}

void APlayerCharacter::AimStarted(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AimStarted");

	if (CombatComp)
	{
		AWeapon* weapon = Cast<AWeapon>(CombatComp->HoldingEquipment);
		if (weapon && weapon->GetCanZoom())
		{
			CombatComp->SetAiming(true);

			// LowReady 상태에서 Aim시 Assault로 바로 변경
			if (_stance == EPlayerStance::EPS_LowReady)
			{
				_stance = EPlayerStance::EPS_Assault;
			}
			
			GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchAimWalkSpeed;

			AdsTimeline.Play();
		}
	}
	
	
}

void APlayerCharacter::AimCompleted(const FInputActionValue& inputValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AimCompleted");

	if (CombatComp)
	{
		AWeapon* weapon = Cast<AWeapon>(CombatComp->HoldingEquipment);
		if (weapon && weapon->GetCanZoom())
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

			AdsTimeline.Reverse();
		}
	}
	
	
}

// void APlayerCharacter::HideCameraIfCharacterClose()
// {
// 	// 카메라가 캐릭터에 너무 가까우면 캐릭터를 안보이게 한다.
// 	if ((CameraComp->GetComponentLocation() - GetActorLocation()).Size() < CameraCloseThreshold)
// 	{
// 		if (CombatComp && CombatComp->HoldingEquipment && CombatComp->HoldingEquipment->GetMesh())
// 		{
// 			GetMesh()->SetVisibility(false);
// 			CombatComp->HoldingEquipment->GetMesh()->bOwnerNoSee = true;
// 		}
// 	}
// 	else
// 	{
// 		if (CombatComp && CombatComp->HoldingEquipment && CombatComp->HoldingEquipment->GetMesh())
// 		{
// 			GetMesh()->SetVisibility(true);
// 			CombatComp->HoldingEquipment->GetMesh()->bOwnerNoSee = false;
// 		}
// 	}
// }

void APlayerCharacter::SetInteractingWeapon(AWeapon* Weapon)
{
	if (InteractingWeapon)
	{
		//->ShowGatherEvidenceWidget(false);
	}

	InteractingWeapon = Weapon;
	if (InteractingWeapon)
	{
		//InteractingWeapon->ShowGatherEvidenceWidget(true);
	}
}

EEquipmentType APlayerCharacter::GetEquipmentType()
{
	if (CombatComp == nullptr || CombatComp->HoldingEquipment == nullptr)
		return EEquipmentType::Max;
	
	return CombatComp->HoldingEquipment->GetEquipmentType();
}

// 플레이어의 입력을 받아서 weapon을 Set한다.
void APlayerCharacter::ChangeEquipment(class AEquipment* Equipment)
{
	if (CombatComp == nullptr || Equipment == nullptr)
		return;

	// 장비를 사용중이면 장비 교체 불가
	if (CombatComp->HoldingEquipment->GetUsing())
		return;

	// 이미 들고있는 무기면 return
	if (GetHoldingEquipment()->GetEquipmentType() == Equipment->GetEquipmentType())
		return;
	
	CombatComp->SwapEquipment(Equipment);
}

bool APlayerCharacter::IsAiming()
{
	return CombatComp && CombatComp->GetAiming();
}

AEquipment* APlayerCharacter::GetHoldingEquipment() const
{
	if (CombatComp == nullptr) return nullptr;

	return CombatComp->HoldingEquipment;
}

FVector APlayerCharacter::GetHitTarget() const
{
	if (CombatComp == nullptr) return FVector();
	return CombatComp->HitTarget;
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



#pragma region AI 데미지 테스트용 처리
//void APlayerCharacter::DamageToEnemy()			// #$!@$##!$#$!@  <<<Enemy 데미지 테스트용>>>  @#$!#!@$!$@#@$#!$!@!$$!$
//
//	// 플레이어 주변에서 가장 가까운 적 찾기
//	AEnemy* closestEnemy = nullptr;
//	float closestDistance = 1000.0f; // 감지 범위 설정

//	for (TActorIterator<AEnemy> It(GetWorld()); It; ++It)
//	{
//		AEnemy* enemy = *It;
//		float distance = FVector::Dist(GetActorLocation(), enemy->GetActorLocation());

//		if (distance < closestDistance)
//		{
//			closestDistance = distance;
//			closestEnemy = enemy;
//		}
//	}

//	if (closestEnemy)
//	{
//		// 데미지 적용 (예: 1만큼 감소)
//		UEnemyFSM* enemyFSM = closestEnemy->FindComponentByClass<UEnemyFSM>();
//		if (enemyFSM)
//		{
//			enemyFSM->OnDamageProcess(1); // 데미지 1 적용
//			PRINT_LOG(TEXT("적에게 데미지를 입힘!"));
//		}
//	}
//}

void APlayerCharacter::DamageToPistolEnemy()
{
	// 플레이어 주변에서 가장 가까운 적 찾기
	APistolEnemy* closestEnemy = nullptr;
	float closestDistance = 1000.0f; // 감지 범위 설정

	for (TActorIterator<APistolEnemy> It(GetWorld()); It; ++It)
	{
		APistolEnemy* enemy = *It;
		float distance = FVector::Dist(GetActorLocation(), enemy->GetActorLocation());

		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestEnemy = enemy;
		}
	}

	if (closestEnemy)
	{
		// 데미지 적용 (예: 1만큼 감소)
		UPistolEnemyFSM* enemyFSM = closestEnemy->FindComponentByClass<UPistolEnemyFSM>();
		if (enemyFSM)
		{
			enemyFSM->OnDamageProcess(1); // 데미지 1 적용
			PRINT_LOG(TEXT("적에게 데미지를 입힘!"));
		}
	}
}


#pragma endregion

