// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModelPluginEditorMode.h"
#include "MyEditorModelPluginEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "MyEditorModelPluginEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/MyEditorModelPluginSimpleTool.h"
#include "Tools/MyEditorModelPluginInteractiveTool.h"

// step 2: register a ToolBuilder in FMyEditorModelPluginEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "MyEditorModelPluginEditorMode"

const FEditorModeID UMyEditorModelPluginEditorMode::EM_MyEditorModelPluginEditorModeId = TEXT("EM_MyEditorModelPluginEditorMode");

FString UMyEditorModelPluginEditorMode::SimpleToolName = TEXT("MyEditorModelPlugin_ActorInfoTool");
FString UMyEditorModelPluginEditorMode::InteractiveToolName = TEXT("MyEditorModelPlugin_MeasureDistanceTool");


UMyEditorModelPluginEditorMode::UMyEditorModelPluginEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UMyEditorModelPluginEditorMode::EM_MyEditorModelPluginEditorModeId,
		LOCTEXT("ModeName", "MyEditorModelPlugin"),
		FSlateIcon(),
		true);
}


UMyEditorModelPluginEditorMode::~UMyEditorModelPluginEditorMode()
{
}


void UMyEditorModelPluginEditorMode::ActorSelectionChangeNotify()
{
}

void UMyEditorModelPluginEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FMyEditorModelPluginEditorModeCommands& SampleToolCommands = FMyEditorModelPluginEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UMyEditorModelPluginSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UMyEditorModelPluginInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UMyEditorModelPluginEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FMyEditorModelPluginEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UMyEditorModelPluginEditorMode::GetModeCommands() const
{
	return FMyEditorModelPluginEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
