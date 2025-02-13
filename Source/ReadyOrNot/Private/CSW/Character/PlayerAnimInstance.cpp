// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerAnimInstance.h"

#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
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
	EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	
	// 방향
	// World -> Local 회전 방향을 구하는 방법.
	// PlayerCharacter의 트랜스폼 변환 행렬을, Velocity에 적용한다.
	FVector InverseTransformDirection = PlayerCharacter->GetActorTransform().InverseTransformVectorNoScale(Velocity);
	Direction = FRotationMatrix::MakeFromX(InverseTransformDirection).Rotator().Yaw;

	EquipmentType = PlayerCharacter->GetEquipmentType();

	PlayerStance = PlayerCharacter->GetPlayerStance();
	bIsCrouched = PlayerCharacter->bIsCrouched;
	bAiming = PlayerCharacter->IsAiming();

	//AO_Yaw = PlayerCharacter->GetAO_Yaw();
	AO_Pitch = PlayerCharacter->GetAO_Pitch();

	// FABRIK IK
	if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() &&
			PlayerCharacter->GetMesh())
	{
		// LeftHandSocket의 WorldTransform
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()
			->GetSocketTransform(FName("LeftHandSocket"), RTS_World);

		// World Space to Bone Relative Space
		//
		FVector outPosition;
		FRotator outRotation;
		
		// 오른손(RightHand)를 기준으로 LeftHandTransform의 상대좌표를 계산하여 반환한다.
		PlayerCharacter->GetMesh()->TransformToBoneSpace(
			FName("RightHand"),
			LeftHandTransform.GetLocation(), FRotator::ZeroRotator,
			outPosition, outRotation
		);

		LeftHandTransform.SetLocation(outPosition);
		LeftHandTransform.SetRotation(FQuat(outRotation));
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, 
				FString::Printf(TEXT("outPosition: X=%.2f, Y=%.2f, Z=%.2f"), outPosition.X, outPosition.Y, outPosition.Z));
    
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, 
				FString::Printf(TEXT("outRotation: Pitch=%.2f, Yaw=%.2f, Roll=%.2f"), outRotation.Pitch, outRotation.Yaw, outRotation.Roll));
		}

	}
}