// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArbetsprovGameMode.h"
#include "ArbetsprovHUD.h"
#include "ArbetsprovCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArbetsprovGameMode::AArbetsprovGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AArbetsprovHUD::StaticClass();
}
