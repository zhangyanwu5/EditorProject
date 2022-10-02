// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MyToolbarPluginStyle.h"

class FMyToolbarPluginCommands : public TCommands<FMyToolbarPluginCommands>
{
public:

	FMyToolbarPluginCommands()
		: TCommands<FMyToolbarPluginCommands>(TEXT("MyToolbarPlugin"), NSLOCTEXT("Contexts", "MyToolbarPlugin", "MyToolbarPlugin Plugin"), NAME_None, FMyToolbarPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
