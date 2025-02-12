// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BTTask_FindPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/NPC.h"
#include "HHS/NPC_AIController.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// attempt to get the NPC's controller
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// attempt to get the balckboard component from the behaviour tree
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			auto const Index = bc->GetValueAsInt(GetSelectedBlackboardKey());

			// get the NPC
			if (auto* npc = Cast<ANPC>(cont->GetPawn()))
			{
				// get the current patrol path vector from the NPC - this is local to the patrol path actor
				auto const Point = npc->GetPatrolPath()->GetPatrolPoint(Index);

				// convert the local vector to a global point
				auto const GlobalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
