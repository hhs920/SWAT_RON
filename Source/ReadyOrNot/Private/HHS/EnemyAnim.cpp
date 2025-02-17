// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/EnemyAnim.h"

#include "Enemy.h"

void UEnemyAnim::AnimNotify_AttackEnd()
{
	if (AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		enemy->FSM->OnAttackEnd();
	}
}

void UEnemyAnim::OnEndAttackAnimation()
{
	bAttackPlay = false;
}
