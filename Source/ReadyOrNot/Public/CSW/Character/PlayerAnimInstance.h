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
	class AEquipment* HoldingEquipment;	

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	EEquipmentType EquipmentType;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	EPlayerStance PlayerStance;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bAiming;
	
	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	float YawOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	float LeanRollValue; // QE 기울기 값
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	float AO_Pitch;

	 // FABRIK
	UPROPERTY(BlueprintReadOnly, Category = FABRIK, meta = (AllowPrivateAccess = true))
	FTransform LeftHandTransform;

	 // RightHandSocket 회전
	UPROPERTY(BlueprintReadOnly, Category = SocketRotation, meta = (AllowPrivateAccess = true))
	FRotator RightHandRotation;
	
};
