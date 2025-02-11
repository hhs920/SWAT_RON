// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Aiming        UMETA(DisplayName = "Aiming"),        // 조준 상태
	EPS_Crouching     UMETA(DisplayName = "Crouching"),     // 웅크리기
	EPS_LowReady      UMETA(DisplayName = "Low Ready"),     // 무기를 아래로 내린 상태 (저준비 자세)
};

UCLASS()
class READYORNOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	friend class UCombatComponent; 
	
	// Sets default values for this character's properties
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Input)
	FVector MoveDir;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = Input)
	float LookUpSpeed = 5.f;

public:

#pragma region 입력

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	class UPlayerInputComponent* InputComp;
	
	void PlayerMove(const FInputActionValue& inputValue);
	void PlayerTurn(const FInputActionValue& inputValue);
	void PlayerLookUp(const FInputActionValue& inputValue);
	
	void PrimaryEquip(const FInputActionValue& inputValue);			// 주무기			1
	void SecondaryEquip(const FInputActionValue& inputValue);		// 보조무기			2
	void GrenadeEquip(const FInputActionValue& inputValue);			// 수류탄			3
	void TacticalEquip(const FInputActionValue& inputValue);		// 전술장비			4
	void LongTacticalEquip(const FInputActionValue& inputValue);	// 긴전술장비		5
	void CableTieEquip(const FInputActionValue& inputValue);		// 케이블타이		6
	
	void LeanLeft(const FInputActionValue& inputValue);				// 기울이기_왼쪽		Q 
	void LeanRight(const FInputActionValue& inputValue);			// 기울이기_오른쪽	E
	
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
	class UCombatComponent* CombatComp;

public:
	EEquipmentType GetEquipmentType();
	bool IsAiming();
	
#pragma endregion

private:
	EPlayerStance PlayerStance;
public:
	FORCEINLINE EPlayerStance GetPlayerStance() const {return PlayerStance;}




	
#pragma region AI 인식
	
private:
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
	
#pragma endregion

	
};
