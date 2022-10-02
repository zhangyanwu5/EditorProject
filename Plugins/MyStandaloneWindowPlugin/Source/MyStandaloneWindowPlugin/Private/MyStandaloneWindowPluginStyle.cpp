// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyStandaloneWindowPluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMyStandaloneWindowPluginStyle::StyleInstance = nullptr;

void FMyStandaloneWindowPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMyStandaloneWindowPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMyStandaloneWindowPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MyStandaloneWindowPluginStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMyStandaloneWindowPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MyStandaloneWindowPluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MyStandaloneWindowPlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MyStandaloneWindowPlugin.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FMyStandaloneWindowPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMyStandaloneWindowPluginStyle::Get()
{
	return *StyleInstance;
}
