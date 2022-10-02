// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyBlueprintLibraryPluginBPLibrary.h"
#include "MyBlueprintLibraryPlugin.h"

UMyBlueprintLibraryPluginBPLibrary::UMyBlueprintLibraryPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UMyBlueprintLibraryPluginBPLibrary::MyBlueprintLibraryPluginSampleFunction(float Param)
{
	return -1;
}

