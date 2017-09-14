// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceApeGameMode.h"
#include "SpaceApePawn.h"

ASpaceApeGameMode::ASpaceApeGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ASpaceApePawn::StaticClass();
}

