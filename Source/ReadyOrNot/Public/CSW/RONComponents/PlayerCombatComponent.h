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

	virtual void SwapEquipment(class AEquipment* Equipment) override;

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
	//void GatherEvidence(class AWeapon* EvidenceToGather);

	void Interact(AActor* ToInteract);

protected:
	virtual void BeginPlay() override;
	virtual void SetUpEquipments() override;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> GrenadeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> TacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> LongTacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> CableTieWeaponClass;

	bool bFireButtonPressed { false };

private:
	float DefaultFOV; // BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	float CurrentFOV; // Weapon의 FOV 관련 세팅값에 따라 동작한다.
	
public:
	FORCEINLINE bool GetAiming() const { return bAiming; }

};