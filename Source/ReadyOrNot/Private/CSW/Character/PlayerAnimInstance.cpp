// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerAnimInstance.h"

#include "CSW/Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter == nullptr)
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	if (PlayerCharacter == nullptr)
		return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	// 방향
	// World -> Local 회전 방향을 구하는 방법.
	// PlayerCharacter의 트랜스폼 변환 행렬을, Velocity에 적용한다.
	FVector InverseTransformDirection = PlayerCharacter->GetActorTransform().InverseTransformVectorNoScale(Velocity);
	Direction = FRotationMatrix::MakeFromX(InverseTransformDirection).Rotator().Yaw;

}