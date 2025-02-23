// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EvidenceUI.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UEvidenceUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EvidenceText;


public:
	void UpdateEvidenceText(int32 Left, int32 Total);
};
