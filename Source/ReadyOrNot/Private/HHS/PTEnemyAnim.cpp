// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/PTEnemyAnim.h"

#include "HHS/PistolEnemy.h"

void UPTEnemyAnim::AnimNotify_AttackEnd()
{
	if (APistolEnemy* enemy = Cast<APistolEnemy>(TryGetPawnOwner()))
	{
		enemy->FSM->OnAttackEnd();
	}
}
