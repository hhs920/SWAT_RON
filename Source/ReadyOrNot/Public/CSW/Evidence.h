// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Evidence.generated.h"

class UWidgetComponent;

UCLASS()
class READYORNOT_API AEvidence : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvidence();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	
	void ShowGatherEvidenceWidget(bool bShowWidget);


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxGathered;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GatherEvidenceWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UUserWidget* GatherEvidenceUI;	

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;
};
