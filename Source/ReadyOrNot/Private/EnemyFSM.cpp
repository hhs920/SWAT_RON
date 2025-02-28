#include "EnemyFSM.h"

#include "AIController.h"
#include "CSW/Character/PlayerCharacter.h"
#include "Enemy.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "ReadyOrNot.h"
#include "VectorTypes.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "CSW/Equipment/Equipment.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/EnemyAnim.h"
#include "Navigation/PathFollowingComponent.h"


UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSPlayer를 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	// ATPSPlayer 타입으로 캐스팅
	target = Cast<APlayerCharacter>(actor);

	// 소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());

	// UEnemyAnim 할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// AIController 할당
	ai = Cast<AAIController>(me->GetController());
	    
	// 이동 속도 설정
	if (me)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
	}
}


void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 실행창에 상태 메세지 출력
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);
	
	switch ( mState )
	{
		case EEnemyState::Idle:		{ IdleState();		}	break;
		case EEnemyState::Move:		{ MoveState();		}	break;
		case EEnemyState::Attack:	{ AttackState();	}	break;
		case EEnemyState::Damage:	{ DamageState();	}	break;
		case EEnemyState::Die:		{ DieState();		}	break;
	}
}

void UEnemyFSM::IdleState()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	
	// 만약 경과 시간이 대기 시간을 초과했다면
	if ( CurrentTime > IdleDelayTime )
	{
		// 새로운 랜덤 위치 가져오기
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		// 이동 상태로 전환
		mState = EEnemyState::Move;
		// 경과 시간 초기화
		CurrentTime = 0.0f;
		anim->animState = mState;
	}
}

void UEnemyFSM::MoveState()
{
	// 타겟 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	
	// 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();
	
	// 월드에 Navigation System을 가져옴
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// 목적지 길찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;

	// 목적지에서 인지할 수 있는 범위
	req.SetAcceptanceRadius(3);
	// 목적지 설정
	req.SetGoalLocation(destination);

	// 길 찾기를 위한 쿼리 생성
	ai->BuildPathfindingQuery(req, query);
	// 길 찾기 결과를 가져오기
	FPathFindingResult r = ns->FindPathSync(query);

	// 길 찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 타겟으로 이동
		ai->MoveToLocation(destination);
	}
	else
	{
		// 랜덤 위치로 이동
		auto result = ai->MoveToLocation(randomPos);
		// 목적지로 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤 위치 가져오기
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}
	
	// 타겟과 거리를 체크해서 attackRange 안으로 들어오면 공격상태로 전환하고 싶다.
	// 거리 체크
	if ( dir.Size() < attackRange )
	{
		// 길 찾기 기능 중지
		ai->StopMovement();
		// 공격 상태로 전환하고 싶다.
		mState = EEnemyState::Attack;
		// 애니메이션 상태 동기화
		anim->animState = mState;
		// 공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;
		// 공격 상태 전환 시 대기 시간이 바로 끝나도록 처리
		CurrentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	// 일정 시간에 한 번씩 공격하고 싶다.
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;	// 위에서도 사용하기 때문에 함수 선언하고 쓰는 것도 좋은 방법
	// 공격 시간이 되면
	if ( CurrentTime > attackDelayTime )
	{
		// 공격을 한다.
		//PRINT_LOG(TEXT("Attack~~!@~#!~#"));
		
		// 경과 시간 초기화
		CurrentTime = 0.0f;
		anim->bAttackPlay = true;

		me->CombatComp->HoldingEquipment->LineTraceTarget = target->GetActorLocation();
		me->CombatComp->HoldingEquipment->BeginUse();
	}
	// 타겟이 공격 범위를 벗어나면 이동 상태로 전환하고 싶다.
	// 1. 타겟과의 거리가 필요하다.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	// 2. 타겟과의 거리가 공격 범위를 벗어남
	if ( distance > attackRange )
	{
		// 새로운 랜덤 위치 가져오기
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		// 3. 상태를 이동 상태로 전환.
		mState = EEnemyState::Move;
		// 4. 애니메이션 상태 동기화
		anim->animState = mState;
	}
}

void UEnemyFSM::DamageState()
{
	// 1. 시간이 흘렀으니까
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if ( CurrentTime > damageDelayTime )
	{
		// 3. 대기 상태로 전환.
		mState = EEnemyState::Idle;
		// 4. 경과시간 초기화
		CurrentTime = 0.0f;
		// 5. 애니메이션 상태 동기화
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
}

void UEnemyFSM::OnDamageProcess(int32 damage)
{
	// 체력 감소
	hp -= damage;
	
	if (ai && target)
	{
		ai->SetFocus(target, EAIFocusPriority::Gameplay);
		PRINT_LOG(TEXT("적이 플레이어를 바라봄"));
	}

	if (hp <= surrenderHP && mState != EEnemyState::Surrender)	// 항복 상태 체크
	{
		mState = EEnemyState::Surrender;
		ai->StopMovement();
		//me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Surrender"));
		if (anim && anim->EnemySurrender)
		{
			me->PlayAnimMontage(anim->EnemySurrender, 1.0f, TEXT("Surrender"));
			me->CombatComp->DropHoldingEquipment(); // 들고있던 장비(총)을 드랍한다.
			PRINT_LOG(TEXT("적이 항복했습니다!"));
		}
		else
		{
			PRINT_LOG(TEXT("Error: EnemyMontage가 없습니다!"));
		}
		return;
	}
	// 만약 체력이 남아있다면
	if ( hp > 0 )
	{
		// 상태를 피격으로 변환
		mState = EEnemyState::Damage;
		
		int32 randValue = FMath::RandRange(0,1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randValue);
		me->PlayAnimMontage(anim->EnemyMontage, 1.0f, FName(*sectionName));
	}
	else
	{
		// 상태를 죽음으로 전환
		mState = EEnemyState::Die;
		// 죽음 애니메이션 재생
		me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Die"));
	}
	// 길찾기 기능 중지
	ai->StopMovement();
	ai->UnPossess();

	// 애니메이션 상태 동기화
	anim->animState = mState;
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	// 월드에 있는 Navigation System을 가져온다.
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation loc;
	// Radius 범위 안에서 도달할 수 있는 랜덤 위치를 가져옴
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemyFSM::OnAttackEnd()
{
	anim->bAttackPlay = false;
}

void UEnemyFSM::SurrenderState()
{
	ai->StopMovement();

	if (anim)
	{
		anim->animState = EEnemyState::Surrender;
	}
	PRINT_LOG(TEXT("surrender!!!!"));
	me->PlayAnimMontage(anim->EnemySurrender, 1.0f, TEXT("Surrender"));
}


