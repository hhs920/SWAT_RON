// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "HHS/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI controller and its npc
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const npc = cont->GetPawn())
		{
			// obtain npc location to use as an origin
			auto const Origin = npc->GetActorLocation();

			// get the navigation system and generate a random location
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin,SearchRadius,Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
