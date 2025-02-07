// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CSW/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
}

// Called every frame
void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInputComponent::SetUpInputMappingContext(AController* Controller)
{
	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		// 서브시스템에 등록
		auto subSystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			// IMC 세팅
			subSystem->AddMappingContext(IMC_PlayerInput, 0);
		}
	}
}


void UPlayerInputComponent::SetUpPlayerInputAction(UInputComponent* PlayerInputComponent)
{
	auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerCharacter == nullptr)
    		PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
	PlayerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, PlayerCharacter, &APlayerCharacter::PlayerMove);
	PlayerInput->BindAction(IA_PlayerTurn, ETriggerEvent::Triggered, PlayerCharacter, &APlayerCharacter::PlayerTurn);
	PlayerInput->BindAction(IA_PlayerLookUp, ETriggerEvent::Triggered, PlayerCharacter, &APlayerCharacter::PlayerLookUp);
}


