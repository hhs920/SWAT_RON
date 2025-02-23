// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

enum class ESelectorState : uint8;
enum class EEquipmentType : uint8;
class USkeletalMeshSocket;
class ACharacter;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
	ACharacter* Character;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage*  FireWeaponMontage;
	
	void PlayFireMontage(bool bAim); 

	void PickUpWeapon(class AWeapon* WeaponToEquip); // Dropped된 무기를 줍는 기능(팀의 무기만 가능)
	
	// 기존에 들고있던 것은 UnEquip하고 손에 들 것을 Equip한다.
	virtual void SwapEquipment(class AEquipment* Equipment);

	void DropHoldingEquipment();
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* Primary;
	UPROPERTY(EditAnywhere, Category = "Combat")
	ESelectorState PrimarySelector;
	
	// 현재 들고있는 무기. 기본으로 주무기 들고있기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
	AEquipment* HoldingEquipment;
	
	virtual void FireButtonPressed(bool bPressed);
	void FireWeaponSetTimer(AWeapon* holdingWeapon);

	virtual void SetUpEquipments();
protected:
	
	void Equip(AEquipment* Equipment);
	void HoldEquipment(AEquipment* EquipmentToAttach);
	void UnholdEquipment();

	UPROPERTY()
	const USkeletalMeshSocket* RightHandSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> PrimaryWeaponClass;

	bool bAiming;
	

};
