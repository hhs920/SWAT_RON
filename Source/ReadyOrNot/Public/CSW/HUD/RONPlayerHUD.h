// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RONPlayerHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UTexture2D* CrosshairsCenter;
	
	// UPROPERTY(VisibleAnywhere)
	// class UTexture2D* CrosshairsTop;
	// UPROPERTY(VisibleAnywhere)
	// class UTexture2D* CrosshairsRight;
	// UPROPERTY(VisibleAnywhere)
	// class UTexture2D* CrosshairsBottom;
	// UPROPERTY(VisibleAnywhere)
	// class UTexture2D* CrosshairsLeft;
};

/**
 * 
 */
UCLASS()
class READYORNOT_API ARONPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();
	
private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter);



public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package;};
};
