
#include "AssetTypeActions_UserDefinedAsset.h"
#include "UserDefinedEditor.h"

void FAssetTypeActions_UserDefinedAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FUserDefinedEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
}
