// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModelPluginEditorModeToolkit.h"
#include "MyEditorModelPluginEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "MyEditorModelPluginEditorModeToolkit"

FMyEditorModelPluginEditorModeToolkit::FMyEditorModelPluginEditorModeToolkit()
{
}

void FMyEditorModelPluginEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FMyEditorModelPluginEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FMyEditorModelPluginEditorModeToolkit::GetToolkitFName() const
{
	return FName("MyEditorModelPluginEditorMode");
}

FText FMyEditorModelPluginEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "MyEditorModelPluginEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
