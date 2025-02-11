// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

enum class EEquipmentType : uint8;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Dropped UMETA(DisplayName = "Dropped State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_Gathered UMETA(DisplayName = "Gathered State"),

	EWS_Max UMETA(DisplayName = "Default MAX")
};

UCLASS()
class READYORNOT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


public:
	void ShowGatherEvidenceWidget(bool bShowWidget);

	/*
	 * Zoomed FOV while aiming
	 */
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 70.f;
	
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;
	
private:
	bool bCanZoom = false;

protected:
	
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EEquipmentType EquipmentType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* GatherEvidenceWidget;

public:
	void SetWeaponState(EWeaponState State);
	void SetWeaponType(EEquipmentType Type);
	void SetCanZoom(bool bEnabled);
	FORCEINLINE USphereComponent* GetAreaSphere() const {	return AreaSphere; }
	FORCEINLINE EEquipmentType GetEquipmentType() const {	return EquipmentType; }
	FORCEINLINE float GetCanZoom() const { return bCanZoom; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
};
