// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UserDefinedAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class MYBLANKPLUGIN_API UUserDefinedAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UUserDefinedAssetFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	
};
