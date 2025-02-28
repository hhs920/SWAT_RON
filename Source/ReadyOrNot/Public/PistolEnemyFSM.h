// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h" // 추가
#include "AITypes.h" // 추가 (FAIRequestID 정의 포함)
#include "PistolEnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)
enum class EPTEnemyState : uint8
{
	Idle UMETA( DisplayName = "대기" ) ,
	Move UMETA( DisplayName = "이동" ), 
	Attack UMETA( DisplayName = "공격" ),
	Damage UMETA( DisplayName = "데미지" ) ,
	Die UMETA( DisplayName = "죽음" ) ,
	Escape UMETA(DisplayName = "도망"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UPistolEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPistolEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EPTEnemyState mState = EPTEnemyState::Idle;
	
	// 대기 상태
	void IdleState();
	
	// 이동 상태
	void MoveState();
	
	// 공격 상태
	void AttackState();
	
	// 피격 상태
	void DamageState();
	
	// 죽음 상태
	void DieState();

	// 도망 상태
	void EscapeState();

	
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
	class APistolEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 500.0f;
	
	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 0.1f;

	// 피격 알림 이벤트 함수
	void OnDamageProcess(int32 damage);

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 hp = 5;

	// 피격 대기 시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;

	// 이동 속도
	UPROPERTY(EditAnywhere, Category=FSM)
	float moveSpeed = 150.0f;

	UPROPERTY()
	class UPTEnemyAnim* anim;

	// Enemy를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 길찾기 수행시 랜덤 위치
	FVector randomPos;
	
	// 랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
	
	void OnAttackEnd();

	// 도망 위치
	FVector escapeLocation;

	// 도망 상태 여부
	bool bIsEscaping = false;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	bool bHasEscaped = false;
};
