// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BTService_IsPlayerInMeleeRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/NPC.h"
#include "HHS/NPC_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"


UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Controller, NPC 가져오기
	auto const * const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto const * const npc = Cast<ANPC>(cont->GetPawn());

	// 플레이어 캐릭터 가져오기
	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);

	// 플레이어가 범위 내에 있는지 여부에 따라 Blackboard 키에 참 or 거짓
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		npc->GetDistanceTo(player) <= MeleeRange);
}
