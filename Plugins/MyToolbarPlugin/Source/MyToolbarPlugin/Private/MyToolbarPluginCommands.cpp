// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyToolbarPluginCommands.h"

#define LOCTEXT_NAMESPACE "FMyToolbarPluginModule"

void FMyToolbarPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MyToolbarPlugin", "Execute MyToolbarPlugin action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
