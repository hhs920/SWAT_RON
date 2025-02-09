// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Primary			UMETA(DisplayName = "주무기"),
	Secondary		UMETA(DisplayName = "보조무기"),
	Grenade			UMETA(DisplayName = "수류탄"),
	Tactical		UMETA(DisplayName = "전술장비"),
	LongTactical	UMETA(DisplayName = "긴 전술장비"),
	CableTie		UMETA(DisplayName = "케이블 타이"),
};

UCLASS()
class READYORNOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
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
	void PlayerCrouch(const FInputActionValue& inputValue);			// 앉기				ctrl
	void Reload(const FInputActionValue& inputValue);				// 재장전			R
	void ChangeSelector(const FInputActionValue& inputValue);		// 조정간			X
	
	void Interact(const FInputActionValue& inputValue);				// 상호작용			F
	
#pragma endregion
#pragma region 스프링암, 카메라

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* CameraComp;
	
#pragma endregion
#pragma region 현재 손에 들고있는 장비

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	EEquipmentType EquipmentType { EEquipmentType::Primary }; // 기본으로 주무기 들고있기

#pragma endregion
#pragma region 상호작용
	
public:
	UPROPERTY(VisibleAnywhere, Category = Interaction)
	class AWeapon* InteractingWeapon;
	
	void SetInteractingWeapon(AWeapon* Weapon);
	
#pragma endregion
#pragma region 전투 컴포넌트

private:
	UPROPERTY(VisibleAnywhere, Category = Combat)
	class UCombatComponent* CombatComp;
	
#pragma endregion
	
};
