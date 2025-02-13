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
	virtual void EquipWeapon(class AWeapon* WeaponToEquip) override;

	void GatherEvidence(class AWeapon* EvidenceToGather);
	
	/**
	 * Aiming and FOV
	 */
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

	FORCEINLINE bool GetAiming() const { return bAiming; }

protected:
	virtual void BeginPlay() override;

	
private:
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> GrenadeWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> TacticalWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> LongTacticalWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> CableTieWeaponClass;


	bool bAiming;
	float DefaultFOV;
	float CurrentFOV;
	
	bool bFireButtonPressed;
};