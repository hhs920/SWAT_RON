// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class USkeletalMeshSocket;
class ACharacter;
class AWeapon;

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Primary			UMETA(DisplayName = "주무기"),
	EET_Secondary		UMETA(DisplayName = "보조무기"),
	EET_Grenade			UMETA(DisplayName = "수류탄"),
	EET_Tactical		UMETA(DisplayName = "전술장비"),
	EET_LongTactical	UMETA(DisplayName = "긴 전술장비"),
	EET_CableTie		UMETA(DisplayName = "케이블 타이"),
	EET_None
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class READYORNOT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetUpInitialEquippedWeapon();

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
	ACharacter* Character;
	
	virtual void Fire();
	// void PickUpWeapon(class AWeapon* WeaponToEquip); // Dropped된 무기를 줍는 기능
	virtual void SetUpEquipments();
	// 기존에 들고있던 것은 UnEquip하고 손에 들 것을 Equip한다.
	virtual void ChangeEquipment(EEquipmentType Type);
	//virtual void AttachEquipmentToSocket(EEquipmentType EquipmentType);
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* Primary;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeapon* Secondary;
	
	// 현재 들고있는 무기. 기본으로 주무기 들고있기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
	AWeapon* EquippedWeapon;

protected:
	UPROPERTY()
	const USkeletalMeshSocket* RightHandSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> PrimaryWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* PrimarySocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> SecondaryWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* SecondarySocket; // 장비가 플레이어에 붙는 위치
	
	bool bAiming;
	

};

