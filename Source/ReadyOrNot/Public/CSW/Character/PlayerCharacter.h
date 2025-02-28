// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "CSW/RONComponents/PlayerCombatComponent.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Assault		  UMETA(DisplayName = "Aiming"),        // 조준 상태
	EPS_Crouching     UMETA(DisplayName = "Crouching"),     // 웅크리기
	EPS_LowReady      UMETA(DisplayName = "Low Ready"),     // 무기를 아래로 내린 상태 (저준비 자세)
};

UCLASS()
class READYORNOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// friend class UPlayerCombatComponent;
	// friend class UCombatComponent; 
	
	// Sets default values for this character's properties
	APlayerCharacter();
	void UpdateHealthHUD();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly)
	class ARONPlayerController* RONPlayerController;
	
	UPROPERTY(EditAnywhere, Category = Input)
	float LookUpSpeed = 5.f;
			
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AssaultWalkSpeed {320.f}; // Assault Stance

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float LowReadyWalkSpeed {400.f}; // LowReady Stance
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AimWalkSpeed {280.f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CrouchWalkSpeed {250.f}; // Crouch Stance
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CrouchAimWalkSpeed {200.f}; // Crouch Stance

	// 기울이기 Q E
	UPROPERTY(VisibleAnywhere, Category = Input)
	float LeanRollValue;
	UPROPERTY(EditAnywhere, Category = Input)
	float MaxLeanRollValue = 15.f;
	UPROPERTY(EditAnywhere, Category = Input)
	float LeanInterpSpeed = 5.f;
	
	FTimerHandle LeanCompletedTimer;

	bool bLeaning = false;


	// ADS (Aiming Down System)
	UPROPERTY(EditAnywhere, Category = "ADS")
	UCurveFloat* AdsCurve;

	FTimeline AdsTimeline; // 델리게이트

	UFUNCTION()
	void OnAdsUpdate(float Alpha);

	UFUNCTION()
	void OnReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		class AController* InstigatorController, AActor* DamageCauser);

	// 플레이어 체력 HP
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	float Health = MaxHealth;

private:
	float LastDamagedTime;
	float DamagedDelayTime = 0.5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Input)
	FVector MoveDir;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 5.f;

	// Aim Offset
	void AimOffset(float DeltaTime);

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

#pragma region 입력
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	class UPlayerInputComponent* InputComp;
	
	void PlayerMove(const FInputActionValue& inputValue);
	void PlayerTurn(const FInputActionValue& inputValue);
	void PlayerLookUp(const FInputActionValue& inputValue);

	void FireStarted(const FInputActionValue& inputValue);
	void FireCompleted(const FInputActionValue& inputValue);

	// void Fire(const FInputActionValue& inputValue);
	
	void PrimaryEquip(const FInputActionValue& inputValue);			// 주무기			1
	void SecondaryEquip(const FInputActionValue& inputValue);		// 보조무기			2
	void GrenadeEquip(const FInputActionValue& inputValue);			// 수류탄			3
	void TacticalEquip(const FInputActionValue& inputValue);		// 전술장비			4
	void LongTacticalEquip(const FInputActionValue& inputValue);	// 긴전술장비		5
	void CableTieEquip(const FInputActionValue& inputValue);		// 케이블타이		6
	
	void LeanStarted(const FInputActionValue& inputValue);					// 기울이기 Q - E 
	void Lean(const FInputActionValue& inputValue);					// 기울이기 Q - E 
	void LeaningResetProcess(); // 기울이기를 끝내기 위한 처리
	void LeanCompleted(const FInputActionValue& inputValue);		// 기울이기 Q - E 
	
	void LowReady(const FInputActionValue& inputValue);				// 로우레디			spacebar
	void CrouchStarted(const FInputActionValue& inputValue);			// 앉기				ctrl
	void CrouchCompleted(const FInputActionValue& inputValue);			// 앉기				ctrl
	void Reload(const FInputActionValue& inputValue);				// 재장전			R
	void ChangeSelector(const FInputActionValue& inputValue);		// 조정간			X
	
	void Interact(const FInputActionValue& inputValue);				// 상호작용			F

	void AimStarted(const FInputActionValue& inputValue);			// 상호작용			RMB
	void AimCompleted(const FInputActionValue& inputValue);			// 상호작용			RMB
	
#pragma endregion
#pragma region 스프링암, 카메라

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* CameraComp;
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

	// UPROPERTY(EditAnywhere)
	// float CameraCloseThreshold = 200.f;
	// void HideCameraIfCharacterClose(); 

#pragma endregion

#pragma region 상호작용
	
public:
	UPROPERTY(VisibleAnywhere, Category = Interaction)
	class AWeapon* InteractingWeapon;
	
	void SetInteractingWeapon(AWeapon* Weapon);
	
#pragma endregion
#pragma region 컴뱃 컴포넌트

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UPlayerCombatComponent* CombatComp;


	EPlayerStance _stance;

public:
	
	EEquipmentType GetEquipmentType();
	void ChangeEquipment(class AEquipment* Equipment);
	bool IsAiming();
	AEquipment* GetHoldingEquipment() const;
	FORCEINLINE EPlayerStance GetPlayerStance() const {return _stance;}
	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;}
	FVector GetHitTarget() const;

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
#pragma region AI 인식
	
private:
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
	//void DamageToEnemy();
	void DamageToPistolEnemy();
#pragma endregion
};
