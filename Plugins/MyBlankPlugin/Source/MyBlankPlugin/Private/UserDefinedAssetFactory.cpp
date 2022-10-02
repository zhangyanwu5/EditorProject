// Fill out your copyright notice in the Description page of Project Settings.


#include "UserDefinedAssetFactory.h"

#include "UserDefinedAsset.h"

UUserDefinedAssetFactory::UUserDefinedAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UUserDefinedAsset::StaticClass();
}

UObject* UUserDefinedAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(UUserDefinedAsset::StaticClass()));
	return NewObject<UUserDefinedAsset>(InParent, InClass, InName, Flags);
}

