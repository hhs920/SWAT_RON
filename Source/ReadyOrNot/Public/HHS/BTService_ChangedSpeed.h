// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangedSpeed.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UBTService_ChangedSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTService_ChangedSpeed();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float Speed = 600.0f;
};
