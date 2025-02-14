// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunEnemyFSM.generated.h"

UENUM(BlueprintType)
enum ERunEnemyState : uint8
{
	Idle UMETA( DisplayName = "대기" ) ,
	Moving UMETA( DisplayName = "이동" ), 
	// UMETA( DisplayName = "공격" ),
	//Damage UMETA( DisplayName = "데미지" ) ,
	//Die UMETA( DisplayName = "죽음" ) ,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API URunEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URunEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM	)
	ERunEnemyState mState = ERunEnemyState::Idle;

	void IdleState();
	void MovingState();

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 2.0f;
	
	// 경과 시간
	float CurrentTime = 0.0f;

	// 타겟
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class APlayerCharacter* target;

	// 소유 액터
	UPROPERTY()
	class ARunEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 500.0f;
	
	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 0.1f;



	
};
