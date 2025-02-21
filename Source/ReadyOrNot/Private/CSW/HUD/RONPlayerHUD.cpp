// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/HUD/RONPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "CSW/HUD/CharacterOverlay.h"

void ARONPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void ARONPlayerHUD::AddCharacterOverlay()
{
	 
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
	
}

void ARONPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		// 뷰포트는 좌상단이 원점이다.
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter = ViewportSize / 2.f;
		if (HUDPackage.CrosshairsCenter)
		{
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
	}
}

void ARONPlayerHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D DrawPoint (ViewportCenter.X - TextureWidth/2.f,
		ViewportCenter.Y - TextureHeight/2.f);

	DrawTexture(Texture,
		DrawPoint.X, DrawPoint.Y,
		TextureWidth, TextureHeight,
		0.f, 0.f,
		1.f, 1.f,
		FLinearColor::White);
}
