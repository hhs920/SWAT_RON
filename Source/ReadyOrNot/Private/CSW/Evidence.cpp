// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Evidence.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "HHS/HHS_GameMode.h"

// Sets default values
AEvidence::AEvidence()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
}

// Called when the game starts or when spawned
void AEvidence::BeginPlay()
{
	Super::BeginPlay();

	if (GatherEvidenceWidget) // UI가 이미 생성되지 않았다면
	{
		// 위젯을 생성하고 변수에 저장
		GatherEvidenceUI = CreateWidget<UUserWidget>(GetWorld(), GatherEvidenceWidget);
        
		if (GatherEvidenceUI)
		{
			GatherEvidenceUI->AddToViewport(); // 화면에 추가
			ShowGatherEvidenceWidget(false);
		}
	}
}

// Called every frame
void AEvidence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEvidence::BeginInteract()
{
	// 박스를 생성한다.
	GetWorld()->SpawnActor<AActor>(BoxGathered, GetActorTransform());

	// 증거를 없앤다.
	if (GatherEvidenceUI)
	{
		GatherEvidenceUI->RemoveFromParent(); // UI 제거
		GatherEvidenceUI = nullptr;
	}

	if (AHHS_GameMode* GM = Cast<AHHS_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		// 증거 수집 개수 +1
		GM->IncreaseWeaponCount();
	}
	
	Destroy();	
}

void AEvidence::EndInteract()
{
}

void AEvidence::ShowGatherEvidenceWidget(bool bShowWidget)
{
	if (bShowWidget)
	{
		if (GatherEvidenceUI)
			GatherEvidenceUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (GatherEvidenceUI)
			GatherEvidenceUI->SetVisibility(ESlateVisibility::Hidden);
	}
}
