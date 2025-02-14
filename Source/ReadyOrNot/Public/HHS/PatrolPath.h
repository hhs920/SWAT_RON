// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class READYORNOT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	// 인덱싱된 순찰 지점 가져오기
	FVector GetPatrolPoint(int const index) const;
	int Num() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;
};
