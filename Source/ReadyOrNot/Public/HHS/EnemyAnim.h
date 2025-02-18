// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 상태 머신 기억 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	EEnemyState animState = EEnemyState::Idle;

	//// 이동 속도
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=TPSPlayerAnimInstance)
	//float Speed = 0.0f;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool bAttackPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	class UAnimMontage* EnemyMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	class UAnimMontage* EnemySurrender;
	
	UFUNCTION()
	void AnimNotify_AttackEnd();
	
	
	// 공격 애니메이션 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

};
