// Copyright Epic Games, Inc. All Rights Reserved.

#include "prototype1GameMode.h"
#include "prototype1PlayerController.h"
#include "prototype1Character.h"
#include "UObject/ConstructorHelpers.h"

Aprototype1GameMode::Aprototype1GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = Aprototype1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}