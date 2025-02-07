// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInputComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere)
	class APlayerCharacter* PlayerCharacter;

	// EnhancedMappingContext, IMC 세팅 
	void SetUpInputMappingContext(class AController* Controller);

	// IMC에 InputAction 맵핑
	void SetUpPlayerInputAction(class UInputComponent* PlayerInputComponent);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputMappingContext* IMC_PlayerInput;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_PlayerTurn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_PlayerLookUp;
};
