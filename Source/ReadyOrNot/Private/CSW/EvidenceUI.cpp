// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/EvidenceUI.h"

#include "Components/TextBlock.h"


void UEvidenceUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEvidenceUI::UpdateEvidenceText(int32 Left, int32 Total)
{
	if (EvidenceText)
	{
		EvidenceText->SetText(FText::Format(NSLOCTEXT("Evidence", "Collected", "수집한 증거 수 ( {0} / {1} )"), Left, Total));
	}
}
