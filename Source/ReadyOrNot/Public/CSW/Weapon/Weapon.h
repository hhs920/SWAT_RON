// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

enum class EEquipmentType : uint8;
class USkeletalMeshComponent;
class USphereComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Dropped UMETA(DisplayName = "Dropped State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_Gathered UMETA(DisplayName = "Gathered State"),

	EWS_Max UMETA(DisplayName = "Default MAX")
};

UENUM(BlueprintType)
enum class ESelectorState : uint8
{
	EST_SemiAuto	UMETA(DisplayName = "단발"),
	EST_Burst		UMETA(DisplayName = "점사"),
	EST_FullAuto	UMETA(DisplayName = "연발")
};

UCLASS()
class READYORNOT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	
	void ShowGatherEvidenceWidget(bool bShowWidget);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/*
	 * 각 무기마다 줌(Aim) 시의 FOV가 다르다.
	 */
	UPROPERTY(EditAnywhere, Category = "FOV")
	float ZoomedFOV = 70.f;
	
	UPROPERTY(EditAnywhere, Category = "FOV")
	float ZoomInterpSpeed = 20.f;
	
	// Zoom 가능한지
	UPROPERTY(EditAnywhere, Category = "FOV", meta = (AllowPrivateAccess = true))
	bool bCanZoom = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	//UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	//USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EEquipmentType EquipmentType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",  meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",  meta = (AllowPrivateAccess = "true"))
	ESelectorState SelectorState;

	//UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	//UWidgetComponent* GatherEvidenceWidget;

	// Get Set
public:
	void SetWeaponState(EWeaponState State);
	void SetWeaponType(EEquipmentType Type);
	void SetCanZoom(bool bEnabled);
	//FORCEINLINE USphereComponent* GetAreaSphere() const {	return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FORCEINLINE EEquipmentType GetEquipmentType() const {	return EquipmentType; }
	FORCEINLINE float GetCanZoom() const { return bCanZoom; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
};
