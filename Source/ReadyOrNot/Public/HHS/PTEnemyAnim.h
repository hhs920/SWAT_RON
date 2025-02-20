// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PistolEnemyFSM.h"
#include "Animation/AnimInstance.h"
#include "PTEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UPTEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	EPTEnemyState AnimState = EPTEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category=FSM)
	bool bAttackPlay;

	UFUNCTION()
	void AnimNotify_AttackEnd();




};
