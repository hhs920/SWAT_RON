// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class USkeletalMeshSocket;

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

public:
	void PickUpWeapon(class AWeapon* WeaponToEquip); // Dropped된 무기를 줍는 기능
	void GatherEvidence(class AWeapon* EvidenceToGather);
	void SetUpEquipments();

	// 기존에 들고있던 것은 UnEquip하고 손에 들 것을 Equip한다.
	void ChangeEquipment(EEquipmentType EquipmentType);
	void AttachEquipmentToSocket(EEquipmentType EquipmentType);

	// 현재 들고있는 무기. 기본으로 주무기 들고있기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
	class AWeapon* EquippedWeapon;
	const USkeletalMeshSocket* HandSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> PrimaryWeaponClass;
	const USkeletalMeshSocket* PrimarySocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> SecondaryWeaponClass;
	const USkeletalMeshSocket* SecondarySocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> GrenadeWeaponClass;
	const USkeletalMeshSocket* GrenadeSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> TacticalWeaponClass;
	const USkeletalMeshSocket* TacticalSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> LongTacticalWeaponClass;
	const USkeletalMeshSocket* LongTacticalSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> CableTieWeaponClass;
	const USkeletalMeshSocket* CableTieSocket; // 장비가 플레이어에 붙는 위치

	/**
	 * Aiming and FOV
	 */
	void SetAiming(bool bIsAiming);
	float DefaultFOV; // BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	float CurrentFOV;
	void InterpFOV(float DeltaTime); // Weapon의 FOV 관련 세팅값에 따라 동작한다.
	


protected:
	void FireButtonPressed(bool bPressed);
	
private:
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
	class APlayerCharacter* Character;
	
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
	bool bFireButtonPressed;

};

