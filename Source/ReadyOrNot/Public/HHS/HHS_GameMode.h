// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/GameCompleteUI.h"
#include "GameFramework/GameModeBase.h"
#include "HHS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API AHHS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void CountAllEvidences();
	void IncreaseWeaponCount();
	void CheckGameComplete();
	void Defeat();

	int32 TotalEvidences;
	int32 CollectedEvidences;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGameCompleteUI> GameCompleteWidgetClass;

	UPROPERTY()
	class UGameCompleteUI* GameCompleteUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEvidenceUI> EvidenceWidgetClass;

	UPROPERTY()
	class UEvidenceUI* EvidenceUI;
};