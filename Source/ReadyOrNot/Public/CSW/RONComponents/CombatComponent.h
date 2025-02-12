// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Primary,
	EET_Secondary,
	EET_Grenade,
	EET_Tactical,
	EET_LongTactical,
	EET_CableTie,
	EET_None
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EquipWeapon(class AWeapon* WeaponToEquip);
	virtual void SetUpEquipments();
	
	// 현재 들고있는 무기. 기본으로 주무기 들고있기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> PrimaryWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> SecondaryWeaponClass;
	
	void Fire();

	UPROPERTY(VisibleAnywhere, Category = Character)
	class ACharacter* Character;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Primary;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Secondary;
	
protected:
	virtual void BeginPlay() override;
};

