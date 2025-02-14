// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/RunEnemyFSM.h"

#include "CSW/Character/PlayerCharacter.h"
#include "HHS/RunEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URunEnemyFSM::URunEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void URunEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSPlayer를 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	// ATPSPlayer 타입으로 캐스팅
	target = Cast<APlayerCharacter>(actor);

	// 소유 객체 가져오기
	me = Cast<ARunEnemy>(GetOwner());
}


// Called every frame
void URunEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 실행창에 상태 메세지 출력
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);

	switch ( mState )
    	{
    		case ERunEnemyState::Idle:		{ IdleState();		}	break;
    		case ERunEnemyState::Moving:	{ MovingState();	}	break;
    	}

}

void URunEnemyFSM::IdleState()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	
	// 만약 경과 시간이 대기 시간을 초과했다면
	if ( CurrentTime > IdleDelayTime )
	{
		// 이동 상태로 전환하고 싶다.
		mState = ERunEnemyState::Moving;
		// 경과 시간 초기화
		CurrentTime = 0.0f;
	}
}

void URunEnemyFSM::MovingState()
{
	
}

