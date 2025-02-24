// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolEnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "ReadyOrNot.h"
#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Equipment/Equipment.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/PistolEnemy.h"
#include "HHS/PTEnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


UPistolEnemyFSM::UPistolEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPistolEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	target = Cast<APlayerCharacter>(actor);
	me = Cast<APistolEnemy>(GetOwner());
	anim = Cast<UPTEnemyAnim>(me->GetMesh()->GetAnimInstance());
	ai = Cast<AAIController>(me->GetController());
	if (me)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
	}
	if (ai)
	{
		ai->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UPistolEnemyFSM::OnMoveCompleted);
	}
}


void UPistolEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, logMsg);
	
	switch ( mState )
	{
	case EPTEnemyState::Idle:		{ IdleState();		}	break;
	case EPTEnemyState::Move:		{ MoveState();		}	break;
	case EPTEnemyState::Attack:		{ AttackState();	}	break;
	case EPTEnemyState::Damage:		{ DamageState();	}	break;
	case EPTEnemyState::Die:		{ DieState();		}	break;
	case EPTEnemyState::Escape:		{ EscapeState();	}	break;
	}
}

void UPistolEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	
	if ( CurrentTime > IdleDelayTime )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EPTEnemyState::Move;
		CurrentTime = 0.0f;
		anim->AnimState = mState;
		
		if (me)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed;  // 원래 속도로 복구
		}
	}
}

void UPistolEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	FPathFindingQuery query;
	FAIMoveRequest req;
	
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);
	
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}
	
	if ( dir.Size() < attackRange )
	{
		ai->StopMovement();
		mState = EPTEnemyState::Attack;
		anim->AnimState = mState;
		anim->bAttackPlay = true;
		CurrentTime = attackDelayTime;
	}
}

void UPistolEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if ( CurrentTime > attackDelayTime )
	{
		PRINT_LOG(TEXT("Attack~~!@~#!~#"));
		
		CurrentTime = 0.0f;
		anim->bAttackPlay = true;
		me->CombatComp->HoldingEquipment->LineTraceTarget = target->GetActorLocation();
		me->CombatComp->HoldingEquipment->BeginUse();
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	
	if ( distance > attackRange )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EPTEnemyState::Move;
		anim->AnimState = mState;
	}
}

void UPistolEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if ( CurrentTime > damageDelayTime )
	{
		mState = EPTEnemyState::Idle;
		CurrentTime = 0.0f;
		anim->AnimState = mState;
	}
}

void UPistolEnemyFSM::DieState()
{
}

void UPistolEnemyFSM::EscapeState()
{
	if (bIsEscaping)
	{
		return;
	}
	bIsEscaping = true;
	PRINT_LOG(TEXT("Escape 상태"));

	
	// 도망 위치 설정 (랜덤 or 특정 위치)
	escapeLocation = FVector(1000.0f, 500.0f, 200.0f); // 예제 좌표
	PRINT_LOG(TEXT("도망 위치: %s"), *escapeLocation.ToString());

	// AI 이동 시작 (한 번만 실행)
	if (ai)
	{
		EPathFollowingRequestResult::Type result = ai->MoveToLocation(escapeLocation);

		if (result == EPathFollowingRequestResult::Failed)
		{
			PRINT_LOG(TEXT("도망 위치로 이동 실패! 새로운 위치 설정"));
			GetRandomPositionInNavMesh(me->GetActorLocation(), 1000.0f, escapeLocation);
			ai->MoveToLocation(escapeLocation);
		}
		else
		{	
			PRINT_LOG(TEXT("적이 도망 중..."));
		}
	}

	// 도망 애니메이션 실행
	if (anim && anim->EnemyRun)
	{
		me->PlayAnimMontage(anim->EnemyRun, 1.0f, TEXT("Escape"));
	}
	// 이동 속도 증가 (예: 원래 속도의 3배로 설정)
	if (me)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed * 3.0f;  // 이동 속도를 증가시킴
	}
	anim->AnimState = mState;

	//// AI가 도망 위치에 도착하면 Idle 상태로 전환
	//if (ai->GetMoveStatus() == EPathFollowingStatus::Idle)
	//{
	//	PRINT_LOG(TEXT("적이 도망 위치에 도착! 대기 상태로 전환"));
	//	mState = EPTEnemyState::Idle;
	//	anim->AnimState = mState;
	//	bIsEscaping = false;
	//}
}

void UPistolEnemyFSM::OnDamageProcess(int32 damage)
{
	hp -= damage;

	if (hp <= 2 && mState != EPTEnemyState::Escape)
	{
		mState = EPTEnemyState::Escape;
		anim->AnimState = mState;
		PRINT_LOG(TEXT("적이 도망칩니다!"));
		return;
	}

	if ( hp > 0 )
	{
		mState = EPTEnemyState::Damage;
		
		int32 randValue = FMath::RandRange(0,1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randValue);
		me->PlayAnimMontage(anim->EnemyMontage, 1.0f, FName(*sectionName));
	}
	else
	{
		mState = EPTEnemyState::Die;
		me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Die"));
	}
	ai->StopMovement();
	anim->AnimState = mState;
}

bool UPistolEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UPistolEnemyFSM::OnAttackEnd()
{
	anim->bAttackPlay = false;
}

void UPistolEnemyFSM::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (mState == EPTEnemyState::Escape)
	{
		PRINT_LOG(TEXT("적이 도망 위치에 도착! Idle 상태로 전환"));

		mState = EPTEnemyState::Idle;
		anim->AnimState = mState;
		bIsEscaping = false;

		// 원래 이동 애니메이션으로 변경
		if (anim && anim->EnemyWalk)
		{
			me->PlayAnimMontage(anim->EnemyWalk, 1.0f, TEXT("Walk"));
		}

		// 이동 속도를 원래대로 복구
		if (me)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
		}
	}
}