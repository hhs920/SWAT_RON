// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/RONComponents/CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class IInteractable;
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
	//void FireWeaponSetTimer(AWeapon* holdingWeapon);

	// Aiming and FOV
	void SetAiming(bool bIsAiming);
	
	// BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	void InterpFOV(float DeltaTime); // Weapon의 FOV 관련 세팅값에 따라 동작한다.
	
	UPROPERTY(VisibleAnywhere, Category = Player)
    class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(VisibleAnywhere, Category = Player)
	class ARONPlayerController* Controller; // 컨트롤러
	UPROPERTY(VisibleAnywhere, Category = Player)
	class ARONPlayerHUD* HUD;	
	
	virtual void FireButtonPressed(bool bPressed) override;

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

	// 바닥에 있는 증거를 수집한다.
	void GatherEvidence(IInteractable* ToInteract);

	IInteractable* interacting;

	FVector HitTarget;

	virtual void SetUpEquipments() override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> SecondaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> GrenadeWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> TacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> LongTacticalWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AWeapon> CableTieWeaponClass;

	bool bFireButtonPressed { false };

	// Line Trace
	void TraceUnderCrossHairs(FHitResult& TraceHitResult);
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceLength  = 80000.f;

	// HUD
	void SetHudCrosshairs(float DeltaTime);
	
private:
	float DefaultFOV; // BeginPlay에서 카메라의 디폴트 FOV값을 설정한다.
	float CurrentFOV; // Weapon의 FOV 관련 세팅값에 따라 동작한다.

	void TraceForEvidence();

	
public:
	FORCEINLINE bool GetAiming() const { return bAiming; }
};
