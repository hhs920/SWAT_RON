// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/HUD/RONPlayerHUD.h"
#include "CSW/Interface/Equipable.h"
#include "CSW/Interface/Usable.h"
#include "CSW/Interface/Interactable.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class USkeletalMeshComponent;
class USkeletalMeshSocket;

UENUM(BlueprintType)
enum class EEquippedState : uint8
{
	Equipped,
	Holding,
	Dropped,
	Gathered,

	Max
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Primary			UMETA(DisplayName = "주무기"),
	Secondary		UMETA(DisplayName = "보조무기"),
	Grenade			UMETA(DisplayName = "수류탄"),
	Tactical		UMETA(DisplayName = "전술장비"),
	LongTactical	UMETA(DisplayName = "긴 전술장비"),
	CableTie		UMETA(DisplayName = "케이블 타이"),

	Max
};

UCLASS(Abstract)
class READYORNOT_API AEquipment : public AActor, public IUsable, public IEquipable, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AEquipment();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class ACharacter* OwnerCharacter;
	
	virtual void Equip() override;
	virtual void Unequip() override;
	
	virtual void BeginUse() override;
	virtual void EndUse() override;
	
	virtual void BeginInteract() override;
	virtual void EndInteract() override;

	//virtual void Drop();

	UPROPERTY(EditDefaultsOnly)
	bool bRotateYtoX = true; // 장비의 방향이 y를 향하고 있으면 x를 향할 수 있도록 켜준다.

	FVector LineTraceTarget;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	FHUDPackage HUDPackage;

	void Drop();

protected:
	virtual void BeginPlay() override;

	virtual void OnBeginEquip() PURE_VIRTUAL(AEquipment::OnBeginEquip, );
	virtual void OnEndEquip() PURE_VIRTUAL(AEquipment::OnEndEquip, );

	virtual void OnBeginUnequip() PURE_VIRTUAL(AEquipment::OnBeginUnequip, );
	virtual void OnEndUnequip() PURE_VIRTUAL(AEquipment::OnEndUnequip, );

	virtual void OnBeginUse() PURE_VIRTUAL(AEquipment::OnBeginUse, );
	virtual void OnEndUse() PURE_VIRTUAL(AEquipment::OnEndUse, );

	virtual void OnBeginInteract() PURE_VIRTUAL(AEquipment::OnBeginInteract, );
	virtual void OnEndInteract() PURE_VIRTUAL(AEquipment::OnEndInteract, );

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	USkeletalMeshComponent* MeshComp;
			
	UPROPERTY()
	class USkeletalMeshSocket* SocketToEquipped; // 장비가 기본으로 붙을 소켓
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	EEquippedState EquippedState;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	EEquipmentType EquipmentType;
	
	UPROPERTY(EditAnywhere, Category = "Interact")
	class UWidgetComponent* InteractWidget;
	
	bool bEquipping;	// 장착 중인지
	bool bEquipped;		// 장착된 장비인지
	bool bUsing;		// 장비 사용중인지

	UPROPERTY(EditDefaultsOnly)
	float DistanceFromADS = 10.f; // ADS 위치로부터의 카메라 거리

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxGathered;

public:
	// GET SET
	FORCEINLINE EEquippedState GetEquippedState() const {return EquippedState;}
	virtual void SetEquippedState(EEquippedState State);

	FORCEINLINE EEquipmentType GetEquipmentType() const {return EquipmentType;}
	virtual void SetEquipmentType(EEquipmentType Type);

	FORCEINLINE USkeletalMeshComponent* GetMesh() const {return MeshComp;}

	FORCEINLINE bool GetEquipping() const {return bEquipping;}
	FORCEINLINE bool GetEquipped() const {return bEquipped;}

	class USkeletalMeshSocket const* GetSocketToEquipped() const;
	
	FORCEINLINE float GetUsing() const { return bUsing; }
	FORCEINLINE float GetDistanceFromADS() const { return DistanceFromADS; }

};
