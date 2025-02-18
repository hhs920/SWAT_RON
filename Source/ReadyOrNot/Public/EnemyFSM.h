#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA( DisplayName = "대기" ) ,
	Move UMETA( DisplayName = "이동" ), 
	Attack UMETA( DisplayName = "공격" ),
	Damage UMETA( DisplayName = "데미지" ) ,
	Die UMETA( DisplayName = "죽음" ) ,
	Surrender UMETA(DisplayName = "항복"),
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;
	
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
	class AEnemy* me;

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
	class UEnemyAnim* anim;

	// Enemy를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 길찾기 수행시 랜덤 위치
	FVector randomPos;
	
	// 랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
	
	void OnAttackEnd();

	// 항복 체력 기준
	UPROPERTY(EditAnywhere, Category=FSM)
	int32 surrenderHP = 1;

	void SurrenderState();
	
};
