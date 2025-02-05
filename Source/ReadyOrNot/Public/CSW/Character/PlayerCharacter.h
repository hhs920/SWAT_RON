// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
#include "InputActionValue.h"

UCLASS()
class READYORNOT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

#pragma region 이동 및 회전
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC_PlayerInput;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_PlayerMove;
	void PlayerMove(const FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_PlayerTurn;
	void PlayerTurn(const FInputActionValue& inputValue);
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_PlayerLookUp;
	void PlayerLookUp(const FInputActionValue& inputValue);
	
#pragma endregion

#pragma region 스프링암, 카메라
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* CameraComp;
	
#pragma region
	
};
