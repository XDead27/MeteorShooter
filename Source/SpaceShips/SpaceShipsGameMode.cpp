// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceShipsGameMode.h"
#include "SpaceShipsCharacter.h"

ASpaceShipsGameMode::ASpaceShipsGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ASpaceShipsCharacter::StaticClass();	
}
