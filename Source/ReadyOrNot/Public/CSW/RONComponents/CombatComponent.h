// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class APlayerCharacter; // PlayerCharacter 전용 컴포넌트
	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void GatherWeaponEvidence(class AWeapon* WeaponToGather);
	
private:
	class APlayerCharacter* PlayerCharacter;
	class AWeapon* EquippedWeapon;
		
};
