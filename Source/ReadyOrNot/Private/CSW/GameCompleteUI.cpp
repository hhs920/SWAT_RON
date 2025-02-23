// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/GameCompleteUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameCompleteUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트 바인딩
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameCompleteUI::OnRestartButtonClicked);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UGameCompleteUI::OnExitButtonClicked);
	}
}

void UGameCompleteUI::OnRestartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TestMap2");
}

void UGameCompleteUI::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UGameCompleteUI::UpdateGameCompleteMessage(const FString& Message)
{
	if (GameCompleteText)
	{
		GameCompleteText->SetText(FText::FromString(Message));
	}
}

