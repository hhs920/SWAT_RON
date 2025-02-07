// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Primary		UMETA(DisplayName = "주무기"),
	Secondary	UMETA(DisplayName = "보조무기"),
	Grenade		UMETA(DisplayName = "수류탄"),
	Tactical	UMETA(DisplayName = "전술장비"),
};

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


#pragma region 이동 및 회전

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
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

	UPROPERTY(VisibleAnywhere, Category = Input)
	FVector MoveDir;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = Input)
	float LookUpSpeed = 5.f;
	
	
#pragma endregion
#pragma region 스프링암, 카메라

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* CameraComp;
	
#pragma endregion
#pragma region 장비

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	EEquipmentType EquipmentType { EEquipmentType::Primary }; // 기본으로 주무기 들고있기

#pragma endregion

	
};
