// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolEnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "ReadyOrNot.h"
#include "CSW/Character/PlayerCharacter.h"
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
	case EPTEnemyState::Attack:	{ AttackState();	}	break;
	case EPTEnemyState::Damage:	{ DamageState();	}	break;
	case EPTEnemyState::Die:		{ DieState();		}	break;
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

void UPistolEnemyFSM::OnDamageProcess(int32 damage)
{
	hp -= damage;
	
	if (hp <= surrenderHP && mState != EPTEnemyState::Surrender)
	{
		mState = EPTEnemyState::Surrender;
		ai->StopMovement();
		//if (anim && anim->EnemySurrender)
		//{
		//	me->PlayAnimMontage(anim->EnemySurrender, 1.0f, TEXT("Surrender"));
		//	PRINT_LOG(TEXT("적이 항복했습니다!"));
		//}
		//else
		//{
		//	PRINT_LOG(TEXT("Error: EnemyMontage가 없습니다!"));
		//}
		//return;
	}

	if ( hp > 0 )
	{
		mState = EPTEnemyState::Damage;
		
		int32 randValue = FMath::RandRange(0,1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randValue);
		//me->PlayAnimMontage(anim->EnemyMontage, 1.0f, FName(*sectionName));
	}
	else
	{
		mState = EPTEnemyState::Die;
		//me->PlayAnimMontage(anim->EnemyMontage, 1.0f, TEXT("Die"));
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

void UPistolEnemyFSM::SurrenderState()
{
	ai->StopMovement();

	//if (anim)
	//{
	//	anim->animState = EPTEnemyState::Surrender;
	//}
	PRINT_LOG(TEXT("surrender!!!!"));
	//me->PlayAnimMontage(anim->EnemySurrender, 1.0f, TEXT("Surrender"));
}

