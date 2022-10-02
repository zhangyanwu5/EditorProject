// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyToolbarPlugin.h"
#include "MyToolbarPluginStyle.h"
#include "MyToolbarPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName MyToolbarPluginTabName("MyToolbarPlugin");

#define LOCTEXT_NAMESPACE "FMyToolbarPluginModule"

void FMyToolbarPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMyToolbarPluginStyle::Initialize();
	FMyToolbarPluginStyle::ReloadTextures();

	FMyToolbarPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMyToolbarPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMyToolbarPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMyToolbarPluginModule::RegisterMenus));
}

void FMyToolbarPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMyToolbarPluginStyle::Shutdown();

	FMyToolbarPluginCommands::Unregister();
}

void FMyToolbarPluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FMyToolbarPluginModule::PluginButtonClicked()")),
							FText::FromString(TEXT("MyToolbarPlugin.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMyToolbarPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMyToolbarPluginCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMyToolbarPluginCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyToolbarPluginModule, MyToolbarPlugin)