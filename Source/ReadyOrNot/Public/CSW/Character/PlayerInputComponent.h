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
	class APlayerCharacter* OwnerCharacter;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Fire;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_PrimaryEquip;		// 주무기			1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_SecondaryEquip;		// 보조무기			2
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_GrenadeEquip;		// 수류탄			3
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_TacticalEquip;		// 전술장비			4
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_LongTacticalEquip;	// 긴전술장비		5
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_CableTieEquip;		// 케이블타이		6

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Lean;			// 기울이기		Q - E

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_LowReady;			// 로우레디			spacebar
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Crouch;				// 앉기				ctrl
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Reload;				// 재장전			R
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_ChangeSelector;		// 조정간			X

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Interact;			// 상호작용			F

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* IA_Aim;			// 상호작용			F

}; 
