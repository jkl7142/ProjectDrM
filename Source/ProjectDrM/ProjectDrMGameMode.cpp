// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectDrMGameMode.h"
#include "ProjectDrMPlayerController.h"
#include "ProjectDrMCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectDrMGameMode::AProjectDrMGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectDrMPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}