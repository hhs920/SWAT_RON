// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BTService_ChangedSpeed.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/NPC.h"

UBTService_ChangedSpeed::UBTService_ChangedSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangedSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto const Cont = OwnerComp.GetAIOwner())
	{
		if (auto* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
