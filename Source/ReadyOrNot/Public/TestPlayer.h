// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestPlayer.generated.h"

UCLASS()
class READYORNOT_API ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly,Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category=Camera)
	class UCameraComponent* TpsCamComp;

	// 총 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComp;
	

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	//// 총알 발사 처리함수
	//void InputFire(const struct FInputActionValue& InputValue);

	// 스나이퍼건 스태틱메시 추가
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* SniperGunMesh;




	// Input
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_PTS;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;


	//// 좌우 회전 입력 처리
	//void Turn(const FInputActionValue& inputValue);
//
	//// 상하 회전 입력 철
	//void LookUp(const FInputActionValue& inputValue);
	

	// 사용자의 좌우 입력을 받아서 이동하고 싶다.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MOVE;

	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "PlayerSetting")
	float WalkSpeed = 600.0f;
	
	// 이동 방향
	FVector Direction;

	//void Move(const FInputActionValue& inputValue);
	

};
