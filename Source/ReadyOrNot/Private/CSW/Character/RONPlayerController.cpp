// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/RONPlayerController.h"

#include "Components/Image.h"
#include "CSW/HUD/CharacterOverlay.h"
#include "CSW/HUD/RONPlayerHUD.h"
#include "HHS/HHS_GameMode.h"

void ARONPlayerController::BeginPlay()
{
	Super::BeginPlay();
	RONPlayerHUD = Cast<ARONPlayerHUD>(GetHUD());
}

void ARONPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	RONPlayerHUD = RONPlayerHUD ? RONPlayerHUD : Cast<ARONPlayerHUD>(GetHUD());
	bool bHUDValid = RONPlayerHUD
		&& RONPlayerHUD->CharacterOverlay
		&& RONPlayerHUD->CharacterOverlay->CharacterImage;
	
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		// 빨갛게 변경
		RONPlayerHUD->CharacterOverlay->CharacterImage->SetColorAndOpacity(
			FColor(255.f, HealthPercent*255.f, HealthPercent*255.f)
		);

		if (HealthPercent <= 0.f)
		{
			AHHS_GameMode* GM = Cast<AHHS_GameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->Defeat();
			}
		}
	}
}

