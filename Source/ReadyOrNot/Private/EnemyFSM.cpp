// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "HHS/FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ReadyOrNot.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSPlayer를 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AFPSCharacter::StaticClass());

	// ATPSPlayer 타입으로 캐스팅
	target = Cast<AFPSCharacter>(actor);

	// 소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());
}


// Called every frame
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
		// 이동 상태로 전환하고 싶다.
		mState = EEnemyState::Move;
		// 경과 시간 초기화
		CurrentTime = 0.0f;
	}
}

void UEnemyFSM::MoveState()
{
	// 타겟 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	
	// 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();

	// dir 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());
	
	// 타겟과 거리를 체크해서 attackRange 안으로 들어오면 공격상태로 전환하고 싶다.
	// 거리 체크
	if ( dir.Size() < attackRange )
	{
		// 공격 상태로 전환하고 싶다.
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	/*
	// 일정 시간에 한 번씩 공격하고 싶다.
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;	// 위에서도 사용하기 때문에 함수 선언하고 쓰는 것도 좋은 방법
	// 공격 시간이 되면
	if ( CurrentTime > attackDelayTime )
	{
	//	// 공격을 한다.
		PRINT_LOG(TEXT("Attack~~!@~#!~#"));
		
		// 경과 시간 초기화
		CurrentTime = 0.0f;
	}
	if ( bPlayed == true )	return;
	// 타겟이 공격 범위를 벗어나면 이동 상태로 전환하고 싶다.
	// 타겟과의 거리가 필요하다.
	*/
	
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	
	// 타겟과의 거리가 공격 범위를 벗어나면
	if ( distance > attackRange )
	{
		// 상태를 이동 상태로 전환하고 싶다.
		mState = EEnemyState::Move;
		CurrentTime = 0.0f;
	}
	else
	{
		// 일정 시간에 한 번씩 공격하고 싶다.
		// 시간이 흐르다가
		CurrentTime += GetWorld()->DeltaTimeSeconds;
		if (CurrentTime > attackDelayTime)
		{
			UAnimInstance* AnimInstance = me->GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				//bPlayed = true;
				//GEngine->AddOnScreenDebugMessage(-1, 5,FColor::Red,TEXT("33"));
				AnimInstance->Montage_Play(me->AM_Slash);
				CurrentTime = 0.0f;

				FVector startPos = me->GetMesh()->GetComponentLocation() + FVector(0, 0, 100);
				FVector endPos = startPos + (me->GetMesh()->GetRightVector() * 150);

				FHitResult hitInfo;

				FCollisionQueryParams params;
				params.AddIgnoredActor(me);
				
				bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_GameTraceChannel1, params);

				if (bHit)
				{
					auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("GunMeshComp"));

					if (enemy)
					{
						UGameplayStatics::ApplyDamage(hitInfo.GetActor(), 1, NULL, NULL, NULL);
					}
				}
				
			}
		}
	}
	/*
	FVector Direction = target->GetActorLocation() - me->GetActorLocation();
	if (Direction.Size() <= attackRange)
	{
		if (CurrentTime > attackDelayTime)
		{
			UAnimInstance* AnimInstance = me->GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(me->AM_Slash);
				CurrentTime = 0.0f;
			}
		}
	}
	else
	{
		mState = EEnemyState::Move;
		CurrentTime = 0.0f;
	}
	*/
}

void UEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if ( CurrentTime > damageDelayTime )
	{
		mState = EEnemyState::Idle;
		CurrentTime = 0.0f;
	}
}

void UEnemyFSM::DieState()
{
}

void UEnemyFSM::OnDamageProcess()
{
	// 체력 감소
	hp--;
	if (hp>0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

