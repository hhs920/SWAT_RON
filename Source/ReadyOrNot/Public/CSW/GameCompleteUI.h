// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCompleteUI.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API UGameCompleteUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

public:
	UPROPERTY(meta = (BindWidget))  
	class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))  
	class UButton* ExitButton; 

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameCompleteText;  // 블루프린트에서 연결할 텍스트


	UFUNCTION(BlueprintCallable, Category = "GameComplete")
	void UpdateGameCompleteMessage(const FString& Message);

	UPROPERTY(EditAnywhere)
	FString RestartMapName = "TestMap2";
};
