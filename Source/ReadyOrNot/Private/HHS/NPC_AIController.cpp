// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/NPC_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "HHS/NPC.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ReadyOrNot/FpsCharacter/ReadyOrNotCharacter.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem() ;
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* const npc = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}


 void ANPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
		// 시야 반경
		SightConfig->SightRadius = 500.0f;
		// 시야 반경 멈추는 반경
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		// 주변 시야 각도 값
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		// 인식 유지 시간
		SightConfig->SetMaxAge(5.f);
		// 자동 성공 범위
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}


void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<APlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
