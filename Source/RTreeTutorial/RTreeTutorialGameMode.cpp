// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTreeTutorialGameMode.h"
#include "RTreeTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARTreeTutorialGameMode::ARTreeTutorialGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
