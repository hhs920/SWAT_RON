// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = true))
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	EEquipmentType EquipmentType;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	EPlayerStance PlayerStance;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bAiming;

	float YawOffset;

	float Lean;
};
