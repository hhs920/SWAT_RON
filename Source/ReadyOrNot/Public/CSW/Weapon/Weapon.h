// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/Equipment/Equipment.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
// class UWidgetComponent;

UENUM(BlueprintType)
enum class ESelectorState : uint8
{
	SemiAuto	UMETA(DisplayName = "단발"),
	Burst		UMETA(DisplayName = "점사"),
	FullAuto	UMETA(DisplayName = "연발"),
	
	MAX UMETA(DisplayName = "DefaultMAX")
};

// UENUM(BlueprintType)
// enum class EFireType : uint8
// {
// 	HitScan UMETA(DisplayName = "Hit Scan Weapon"),
// 	Projectile UMETA(DisplayName = "Projectile Weapon"),
// 	Shotgun UMETA(DisplayName = "Shotgun Weapon"),
//
// 	MAX UMETA(DisplayName = "DefaultMAX")
// };

UCLASS(Abstract)
class READYORNOT_API AWeapon : public AEquipment
{
	GENERATED_BODY()
	
public:
	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	
	FHitResult HitResult; // TODO : 사용될 때 HitTarget을 계산해야한다.
	FVector TraceEndWithScatter(const FVector& HitTarget);

	void ShowGatherEvidenceWidget(bool bShowWidget);

	// virtual void Reload();

	UPROPERTY()
	FTimerHandle FireTimer;

	virtual void Fire(FVector& HitTarget); // public Use 에서 사용된다. 

	 // 캐릭터에 의해 Drop될 때 호출된다.
	
protected:
	virtual void BeginPlay() override;

	virtual void OnBeginEquip() override;
	virtual void OnEndEquip() override; // 캐릭터에 의해 Equip될 때 호출된다.
	
	virtual void OnBeginUnequip() override;
	virtual void OnEndUnequip() override;
	
	virtual void OnBeginUse() override;
	virtual void OnEndUse() override;

	virtual void OnBeginInteract() override;
	virtual void OnEndInteract() override;

	virtual void Reload();

	bool bReloading;
	
	UPROPERTY(EditAnywhere, Category = "FOV")
	float ZoomedFOV = 70.f;
	
	UPROPERTY(EditAnywhere, Category = "FOV")
	float ZoomInterpSpeed = 20.f;
	
	// Zoom 가능한지
	UPROPERTY(EditAnywhere, Category = "FOV", meta = (AllowPrivateAccess = true))
	bool bCanZoom = true;

	// 조정간
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ESelectorState SelectorState;
	UPROPERTY()
	TArray<ESelectorState> AvailableSelectorStates;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bUseSemiAuto = true; // 단발
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bUseBurst = false; //점사
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 MaxBurstCount = 3; // 점사 발수
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 BurstFireCount = 0;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bUseFullAuto = false; // 연발
	
	UPROPERTY()
	const USkeletalMeshSocket* AmmoEjectSocket; // 총알이 발사되는 소켓
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UAnimationAsset* FireAnim;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UAnimationAsset* ReloadAnim;


	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float AttackDistance = 10000.f; // 100m
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bUseScatter = false;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float Damage = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float HeadShotDamage = 3.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass; // 탄피

	UPROPERTY(EditAnywhere)
	int32 Ammo;
	
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GatherEvidenceWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UUserWidget* GatherEvidenceUI;

public:
	bool IsEmpty();
	// bool IsFull();
	FORCEINLINE void SetBurstFireCount(int32 cnt);
	
	// GET SET
	FORCEINLINE ESelectorState GetSelectorState() const {	return SelectorState; }
	void ChangeSelectorState();
	void SetSelectorState(ESelectorState state);
	
	//FORCEINLINE USphereComponent* GetAreaSphere() const {	return AreaSphere; }
	
	FORCEINLINE float GetCanZoom() const { return bCanZoom; }
	
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	
	FORCEINLINE float GetFireDelay() const { return FireDelay; }

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }

	FORCEINLINE float GetBurstFireCount() const { return BurstFireCount; }
	FORCEINLINE float GetMaxBurstCount() const { return MaxBurstCount; }
	
};
