// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/HHS_GameMode.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CSW/Evidence.h"
#include "CSW/EvidenceUI.h"
#include "CSW/Weapon/ProjectileWeapon.h"
#include "CSW/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"


void AHHS_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (EvidenceWidgetClass)
	{
		EvidenceUI = CreateWidget<UEvidenceUI>(GetWorld(), EvidenceWidgetClass);
		if (EvidenceUI)
		{
			EvidenceUI->AddToViewport();
			EvidenceUI->SetVisibility(ESlateVisibility::Visible);
			
			CountAllEvidences();
		}
	}
	

	if (GameCompleteWidgetClass)
	{
		GameCompleteUI = CreateWidget<UGameCompleteUI>(GetWorld(), GameCompleteWidgetClass);
		if (GameCompleteUI)
		{
			GameCompleteUI->AddToViewport();
			GameCompleteUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AHHS_GameMode::CountAllEvidences()
{
	UWorld* World = GetWorld();
	if (!World) return;

	CollectedEvidences = 0;

	// 수집 대상 개수 확인 및 저장
	TArray<AActor*> Weapons;
	TArray<AActor*> Evidences;

	UGameplayStatics::GetAllActorsOfClass(World, AProjectileWeapon::StaticClass(), Weapons);
	UE_LOG(LogTemp, Warning, TEXT("Weapons.Num(): %d"), Weapons.Num());
	
	TotalEvidences += Weapons.Num() - 2;  // 플레이어가 사용하는 무기 2개 제외
	

	UGameplayStatics::GetAllActorsOfClass(World, AEvidence::StaticClass(), Evidences);
	UE_LOG(LogTemp, Warning, TEXT("Evidences.Num(): %d"), Evidences.Num());

	TotalEvidences += Evidences.Num();
    UE_LOG(LogTemp, Warning, TEXT("Total Evidences: %d"), TotalEvidences);


	EvidenceUI->UpdateEvidenceText(CollectedEvidences, TotalEvidences);
}

void AHHS_GameMode::IncreaseWeaponCount()
{
	CollectedEvidences++;
	EvidenceUI->UpdateEvidenceText(CollectedEvidences, TotalEvidences);
	
	CheckGameComplete();
}

void AHHS_GameMode::CheckGameComplete()
{
	// 증거를 덜 수집했으면 return
	if (CollectedEvidences < TotalEvidences)
		return;

	// 증거를 다 수집했으면
	if (GameCompleteWidgetClass && GameCompleteUI)
	{
		GameCompleteUI->UpdateGameCompleteMessage("Mission Complete");
		GameCompleteUI->SetVisibility(ESlateVisibility::Visible); // UI 띄우기
		
		
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			UGameplayStatics::SetGamePaused( GetWorld() , true ); // 게임 일시정지
			PlayerController->SetInputMode(FInputModeUIOnly()); // UI 모드로 변경
			PlayerController->SetShowMouseCursor(true);          // 마우스 커서 표시
		}
	}
}

void AHHS_GameMode::Defeat()
{
	if (GameCompleteWidgetClass && GameCompleteUI)
	{
		GameCompleteUI->UpdateGameCompleteMessage("Mission failed");
		GameCompleteUI->SetVisibility(ESlateVisibility::Visible); // UI 띄우기
		
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			UGameplayStatics::SetGamePaused( GetWorld() , true ); // 게임 일시정지
			PlayerController->SetInputMode(FInputModeUIOnly()); // UI 모드로 변경
			PlayerController->SetShowMouseCursor(true);         // 마우스 커서 표시
		}
	}
}

