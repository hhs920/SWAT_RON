// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetUpEquipments() override;
	virtual void ChangeEquipment(EEquipmentType Type) override;

	virtual void Fire() override;
	
	// Aiming and FOV
	void SetAiming(bool bIsAiming);
	
	// BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	void InterpFOV(float DeltaTime); // Weapon의 FOV 관련 세팅값에 따라 동작한다.
	
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
    class APlayerCharacter* PlayerCharacter;
	
	void FireButtonPressed(bool bPressed);

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Grenade;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* Tactical;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* LongTactical;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class AWeapon* CableTie;

	// 바닥에 있는 증거 무기를 수집한다.
	void GatherEvidence(class AWeapon* EvidenceToGather);

protected:
	virtual void BeginPlay() override;
	virtual void SetUpInitialEquippedWeapon() override;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> GrenadeWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* GrenadeSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> TacticalWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* TacticalSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> LongTacticalWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* LongTacticalSocket; // 장비가 플레이어에 붙는 위치

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> CableTieWeaponClass;
	UPROPERTY()
	const USkeletalMeshSocket* CableTieSocket; // 장비가 플레이어에 붙는 위치

	bool bFireButtonPressed { false };

private:
	float DefaultFOV; // BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	float CurrentFOV; // Weapon의 FOV 관련 세팅값에 따라 동작한다.
	
public:
	FORCEINLINE bool GetAiming() const { return bAiming; }

};