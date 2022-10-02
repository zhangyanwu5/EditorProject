#pragma once

#include "AssetTypeActions_Base.h"
#include "UserDefinedAsset.h"


class FAssetTypeActions_UserDefinedAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_UserDefinedAsset()
	{
	}
	
	FAssetTypeActions_UserDefinedAsset(EAssetTypeCategories::Type InAssetCategoryBit)
		: AssetCategoryBit(InAssetCategoryBit)
	{
	}
	
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_UserDefinedAsset", "User Defined Asset"); }
	virtual FColor GetTypeColor() const override { return FColor( 255, 0, 0 ); }
	virtual UClass* GetSupportedClass() const override { return UUserDefinedAsset::StaticClass(); }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	virtual uint32 GetCategories() override { return AssetCategoryBit; }

private:

	EAssetTypeCategories::Type AssetCategoryBit;
};
