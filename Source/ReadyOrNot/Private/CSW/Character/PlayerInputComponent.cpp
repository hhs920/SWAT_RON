// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Character/PlayerInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CSW/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(TEXT("/Game/CSW/Input/IA_PlayerMove.IA_PlayerMove"));
    if (MoveAction.Succeeded()) { IA_PlayerMove = MoveAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> TurnAction(TEXT("/Game/CSW/Input/IA_PlayerTurn.IA_PlayerTurn"));
    if (TurnAction.Succeeded()) { IA_PlayerTurn = TurnAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> LookUpAction(TEXT("/Game/CSW/Input/IA_PlayerLookUp.IA_PlayerLookUp"));
    if (LookUpAction.Succeeded()) { IA_PlayerLookUp = LookUpAction.Object; }

	static ConstructorHelpers::FObjectFinder<UInputAction> FireAction(TEXT("/Game/CSW/Input/IA_Fire.IA_Fire"));
	if (FireAction.Succeeded()) { IA_Fire = FireAction.Object; }


    static ConstructorHelpers::FObjectFinder<UInputAction> PrimaryEquipAction(TEXT("/Game/CSW/Input/IA_PrimaryEquip.IA_PrimaryEquip"));
    if (PrimaryEquipAction.Succeeded()) { IA_PrimaryEquip = PrimaryEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> SecondaryEquipAction(TEXT("/Game/CSW/Input/IA_SecondaryEquip.IA_SecondaryEquip"));
    if (SecondaryEquipAction.Succeeded()) { IA_SecondaryEquip = SecondaryEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> GrenadeEquipAction(TEXT("/Game/CSW/Input/IA_GrenadeEquip.IA_GrenadeEquip"));
    if (GrenadeEquipAction.Succeeded()) { IA_GrenadeEquip = GrenadeEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> TacticalEquipAction(TEXT("/Game/CSW/Input/IA_TacticalEquip.IA_TacticalEquip"));
    if (TacticalEquipAction.Succeeded()) { IA_TacticalEquip = TacticalEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> LongTacticalEquipAction(TEXT("/Game/CSW/Input/IA_LongTacticalEquip.IA_LongTacticalEquip"));
    if (LongTacticalEquipAction.Succeeded()) { IA_LongTacticalEquip = LongTacticalEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> CableTieEquipAction(TEXT("/Game/CSW/Input/IA_CableTieEquip.IA_CableTieEquip"));
    if (CableTieEquipAction.Succeeded()) { IA_CableTieEquip = CableTieEquipAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> LeanAction(TEXT("/Game/CSW/Input/IA_Lean.IA_Lean"));
    if (LeanAction.Succeeded()) { IA_Lean = LeanAction.Object; }


    static ConstructorHelpers::FObjectFinder<UInputAction> LowReadyAction(TEXT("/Game/CSW/Input/IA_LowReady.IA_LowReady"));
    if (LowReadyAction.Succeeded()) { IA_LowReady = LowReadyAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> CrouchAction(TEXT("/Game/CSW/Input/IA_Crouch.IA_Crouch"));
    if (CrouchAction.Succeeded()) { IA_Crouch = CrouchAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> ReloadAction(TEXT("/Game/CSW/Input/IA_Reload.IA_Reload"));
    if (ReloadAction.Succeeded()) { IA_Reload = ReloadAction.Object; }

    static ConstructorHelpers::FObjectFinder<UInputAction> ChangeSelectorAction(TEXT("/Game/CSW/Input/IA_ChangeSelector.IA_ChangeSelector"));
    if (ChangeSelectorAction.Succeeded()) { IA_ChangeSelector = ChangeSelectorAction.Object; }

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractAction(TEXT("/Game/CSW/Input/IA_Interact.IA_Interact"));
	if (InteractAction.Succeeded()) { IA_Interact = InteractAction.Object; }

	static ConstructorHelpers::FObjectFinder<UInputAction> AimAction(TEXT("/Game/CSW/Input/IA_Aim.IA_Aim"));
	if (AimAction.Succeeded()) { IA_Aim = AimAction.Object; }
	
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

	if (IMC_PlayerInput == nullptr)
	{
		
		FSoftObjectPath IMCPath(TEXT("/Game/CSW/Input/IMC_PlayerInput.IMC_PlayerInput"));
		IMC_PlayerInput = Cast<UInputMappingContext>(IMCPath.TryLoad());
	}
	
	PlayerInput->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerMove);
	PlayerInput->BindAction(IA_PlayerTurn, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerTurn);
	PlayerInput->BindAction(IA_PlayerLookUp, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::PlayerLookUp);

	PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::FireStarted);
	PlayerInput->BindAction(IA_Fire, ETriggerEvent::Completed, OwnerCharacter, &APlayerCharacter::FireCompleted);

	PlayerInput->BindAction(IA_PrimaryEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::PrimaryEquip);
	PlayerInput->BindAction(IA_SecondaryEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::SecondaryEquip);
	PlayerInput->BindAction(IA_GrenadeEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::GrenadeEquip);
	PlayerInput->BindAction(IA_TacticalEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::TacticalEquip);
	PlayerInput->BindAction(IA_LongTacticalEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::LongTacticalEquip);
	PlayerInput->BindAction(IA_CableTieEquip, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::CableTieEquip);

	PlayerInput->BindAction(IA_Lean, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::LeanStarted);
	PlayerInput->BindAction(IA_Lean, ETriggerEvent::Triggered, OwnerCharacter, &APlayerCharacter::Lean);
	PlayerInput->BindAction(IA_Lean, ETriggerEvent::Completed, OwnerCharacter, &APlayerCharacter::LeanCompleted);
	PlayerInput->BindAction(IA_Lean, ETriggerEvent::Canceled, OwnerCharacter, &APlayerCharacter::LeanCompleted);
	
	PlayerInput->BindAction(IA_LowReady, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::LowReady);
	
	PlayerInput->BindAction(IA_Crouch, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::CrouchStarted);
	PlayerInput->BindAction(IA_Crouch, ETriggerEvent::Completed, OwnerCharacter, &APlayerCharacter::CrouchCompleted);
	
	PlayerInput->BindAction(IA_Reload, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::Reload);
	PlayerInput->BindAction(IA_ChangeSelector, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::ChangeSelector);

	PlayerInput->BindAction(IA_Interact, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::Interact);
	
	PlayerInput->BindAction(IA_Aim, ETriggerEvent::Started, OwnerCharacter, &APlayerCharacter::AimStarted);
	PlayerInput->BindAction(IA_Aim, ETriggerEvent::Completed, OwnerCharacter, &APlayerCharacter::AimCompleted);
}
