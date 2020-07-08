// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ConwayGameMode.h"
#include "ConwayHUD.h"
#include "ConwayCharacter.h"
#include "UObject/ConstructorHelpers.h"

AConwayGameMode::AConwayGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AConwayHUD::StaticClass();
}
