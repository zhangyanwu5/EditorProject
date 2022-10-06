#pragma once

#include "Toolkits/AssetEditorToolkit.h"

class IDetailsView;

class FUserDefinedEditor : public FAssetEditorToolkit
{
public:

	DECLARE_DELEGATE_RetVal_OneParam(TArray<UObject*>, FGetDetailsViewObjects, const TArray<UObject*>&);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit, FGetDetailsViewObjects GetDetailsViewObjects);

	virtual ~FUserDefinedEditor();

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual bool IsPrimaryEditor() const override{return true;}
	virtual bool IsSimpleAssetEditor() const override {return true;}

	virtual void PostRegenerateMenusAndToolbars() override;

	void SetPropertyVisibilityDelegate(FIsPropertyVisible InVisibilityDelegate);
	void SetPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled InPropertyEditingDelegate);

private:

	TSharedRef<SDockTab> SpawnPropertiesTab(const FSpawnTabArgs& Args);

	void HandleAssetPostImport(class UFactory* InFactory, UObject* InObject);

	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	TSharedPtr<class IDetailsView> DetailsView;

	static const FName UserDefinedEditorAppIdentifier;

	static const FName PropertiesTabId;

	TArray<UObject*> EditingObjects;

public:

	static const FName ToolkitFName;

	static TSharedRef<FUserDefinedEditor> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit, FGetDetailsViewObjects GetDetailsViewObjects = FGetDetailsViewObjects());
	static TSharedRef<FUserDefinedEditor> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*> ObjectsToEdit, FGetDetailsViewObjects GetDetailsViewObjects = FGetDetailsViewObjects());
	
};
