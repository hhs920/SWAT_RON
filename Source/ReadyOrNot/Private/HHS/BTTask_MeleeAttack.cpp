// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BTTask_MeleeAttack.h"
#include "HHS/NPC.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 범위 벗어나면 플레이어 공격 안함
	auto const OutOfRange = ! OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		// Task 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	// 범위 내에 있으면 AI의 컨트롤러와 NPC 가져옴
	auto const * const cont = OwnerComp.GetAIOwner();
	auto * const npc = Cast<ANPC>(cont->GetPawn());
	
	// 	NPC가 IComnatInterface를 지원하면 Execute_MeleeAttack 함수 호출
	if (auto* const icombat = Cast<ICombatInterface>(npc))
	{
		// 몽타주가 완성되었는지 확인하기 위해 필요. (다시 플레이하지 않도록 하기 위해서)
		if (MontageHasFinished(npc))
		{
			icombat->Execute_MeleeAttack(npc);
		}
	}
	// Task 종료
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(ANPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
	
}
































