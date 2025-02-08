// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReadyOrNotGameMode.h"
#include "ReadyOrNotCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReadyOrNotGameMode::AReadyOrNotGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
