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

	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
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

	if (OwnerCharacter == nullptr)
    		OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	
	PlayerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerMove);
	PlayerInput->BindAction(IA_PlayerTurn, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerTurn);
	PlayerInput->BindAction(IA_PlayerLookUp, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerLookUp);
	PlayerInput->BindAction(IA_PrimaryEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PrimaryEquip);
	PlayerInput->BindAction(IA_SecondaryEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::SecondaryEquip);
	PlayerInput->BindAction(IA_GrenadeEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::GrenadeEquip);
	PlayerInput->BindAction(IA_TacticalEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::TacticalEquip);
	PlayerInput->BindAction(IA_LongTacticalEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::LongTacticalEquip);
	PlayerInput->BindAction(IA_CableTieEquip, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::CableTieEquip);
	PlayerInput->BindAction(IA_LeanLeft, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::LeanLeft);
	PlayerInput->BindAction(IA_LeanRight, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::LeanRight);
	PlayerInput->BindAction(IA_LowReady, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::LowReady);
	PlayerInput->BindAction(IA_SitDown, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::SitDown);
	PlayerInput->BindAction(IA_Reload, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::Reload);
	PlayerInput->BindAction(IA_ChangeSelector, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::ChangeSelector);
}
