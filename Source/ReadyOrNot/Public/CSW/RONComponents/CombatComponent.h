// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Primary			UMETA(DisplayName = "주무기"),
	EET_Secondary		UMETA(DisplayName = "보조무기"),
	EET_Grenade			UMETA(DisplayName = "수류탄"),
	EET_Tactical		UMETA(DisplayName = "전술장비"),
	EET_LongTactical	UMETA(DisplayName = "긴 전술장비"),
	EET_CableTie		UMETA(DisplayName = "케이블 타이"),
	None
};

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
	void GatherEvidence(class AWeapon* EvidenceToGather);
	void SetUpEquipments();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	EEquipmentType EquipmentType { EEquipmentType::EET_Primary }; // 기본으로 주무기 들고있기

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> PrimaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> SecondaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> GrenadeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> TacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> LongTacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> CableTieWeaponClass;
	
private:
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
	class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Primary;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Secondary;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Grenade;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Tactical;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* LongTactical;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* CableTie;
	
	bool bAiming;
	
};

