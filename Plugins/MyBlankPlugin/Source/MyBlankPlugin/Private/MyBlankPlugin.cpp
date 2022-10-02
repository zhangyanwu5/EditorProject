// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyBlankPlugin.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_UserDefinedAsset.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FMyBlankPluginModule"

void FMyBlankPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type AssetCategoryBit = AssetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("UserDefinedAsset")), LOCTEXT("UserDefinedAssetCategory", "User Defined Asset"));
	TSharedPtr<FAssetTypeActions_UserDefinedAsset> AssetTypeAction = MakeShareable(new FAssetTypeActions_UserDefinedAsset(AssetCategoryBit));
	AssetToolsModule.RegisterAssetTypeActions(AssetTypeAction.ToSharedRef());
}

void FMyBlankPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetToolsModule.UnregisterAssetTypeActions(MakeShareable(new FAssetTypeActions_UserDefinedAsset));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyBlankPluginModule, MyBlankPlugin)