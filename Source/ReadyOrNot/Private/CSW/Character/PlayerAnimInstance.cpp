// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerAnimInstance.h"

#include "CSW/Character/PlayerCharacter.h"
#include "CSW/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	HoldingEquipment = PlayerCharacter->GetHoldingEquipment();
	
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

	
	if (HoldingEquipment && HoldingEquipment->GetMesh() && PlayerCharacter->GetMesh())
	{
		// FABRIK IK
			// LeftHandSocket의 WorldTransform
		LeftHandTransform = HoldingEquipment->GetMesh()
			->GetSocketTransform(FName("LeftHandSocket"), RTS_World);

			// World Space to Bone Relative Space
		FVector outPosition;
		FRotator outRotation;
		
			// 오른손(RightHand)를 기준으로 LeftHandTransform의 상대좌표를 계산하여 반환한다.
		PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("RightHand"),
			LeftHandTransform.GetLocation(), FRotator::ZeroRotator,
			outPosition, outRotation
		);
		LeftHandTransform.SetLocation(outPosition);
		LeftHandTransform.SetRotation(FQuat(outRotation));

		// 총구 방향 조정
		//FTransform RightHandTr = HoldingEquipment->GetMesh()->GetSocketTransform(FName("RightHandSocket"));
		//	// ABP에서 RightHandRotation로 적용해서 Transform Modify Bone 노드를 실행한다.
		//RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTr.GetLocation(), PlayerCharacter->GetHitTarget());
		//
		//FTransform MuzzleTipTransform = HoldingEquipment->GetMesh()->GetSocketTransform(FName("MuzzleFlash"),
		//	ERelativeTransformSpace::RTS_World);
		//	// MuzzleFlash소켓의 X방향
		//FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
		//DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(),  MuzzleTipTransform.GetLocation()+MuzzleX * 1000.f, FColor::Red);
		//DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), PlayerCharacter->GetHitTarget(), FColor::Orange);
	}

	// Leaning
	LeanRollValue = PlayerCharacter->LeanRollValue;
}