// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModelPluginModule.h"
#include "MyEditorModelPluginEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "MyEditorModelPluginModule"

void FMyEditorModelPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMyEditorModelPluginEditorModeCommands::Register();
}

void FMyEditorModelPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FMyEditorModelPluginEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyEditorModelPluginModule, MyEditorModelPluginEditorMode)