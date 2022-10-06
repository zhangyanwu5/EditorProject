#include "UserDefinedEditor.h"
#include "SourceCodeNavigation.h"
#include "Widgets/Input/SHyperlink.h"

#define LOCTEXT_NAMESPACE "UserDefinedEditor"

const FName FUserDefinedEditor::ToolkitFName(TEXT("UserDefinedAssetEditor"));
const FName FUserDefinedEditor::PropertiesTabId(TEXT("UserDefinedEditor_Properties"));

void FUserDefinedEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_UserDefinedAssetEditor", "Asset Editor"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FUserDefinedEditor::SpawnPropertiesTab))
		.SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FUserDefinedEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

const FName FUserDefinedEditor::UserDefinedEditorAppIdentifier(TEXT("UserDefinedEditorApp"));

FUserDefinedEditor::~FUserDefinedEditor()
{
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.RemoveAll(this);
	FCoreUObjectDelegates::OnObjectsReplaced.RemoveAll(this);

	DetailsView.Reset();
}

void FUserDefinedEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit, FGetDetailsViewObjects GetDetailsViewObjects)
{
	EditingObjects = ObjectsToEdit;
	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.AddSP(this, &FUserDefinedEditor::HandleAssetPostImport);
	FCoreUObjectDelegates::OnObjectsReplaced.AddSP(this, &FUserDefinedEditor::OnObjectsReplaced);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_UserDefinedAssetEditor_Layout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->Split
			(
				FTabManager::NewStack()
				->AddTab(PropertiesTabId, ETabState::OpenedTab)
			)
		)
	);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FUserDefinedEditor::UserDefinedEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit);

	RegenerateMenusAndToolbars();

	const TArray<UObject*> ObjectsToEditInDetailsView = (GetDetailsViewObjects.IsBound()) ? GetDetailsViewObjects.Execute(ObjectsToEdit) : ObjectsToEdit;

	for (UObject* ObjectToEditInDetailsView : ObjectsToEditInDetailsView)
	{
		ObjectToEditInDetailsView->SetFlags(RF_Transactional);
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObjects(ObjectsToEditInDetailsView);
	}
}

FName FUserDefinedEditor::GetToolkitFName() const
{
	return ToolkitFName;
}

FText FUserDefinedEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "User Defined Asset Editor");
}

FText FUserDefinedEditor::GetToolkitName() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();

	check(EditingObjs.Num() > 0);

	FFormatNamedArguments Args;
	Args.Add(TEXT("ToolkitName"), GetBaseToolkitName());

	if (EditingObjs.Num() == 1)
	{
		const UObject* EditingObject = EditingObjs[0];
		return FText::FromString(EditingObject->GetName());
	}
	else
	{
		UClass* SharedBaseClass = nullptr;
		for (int32 x = 0; x < EditingObjs.Num(); ++x)
		{
			UObject* Obj = EditingObjs[x];
			check(Obj);

			UClass* ObjClass = Cast<UClass>(Obj);
			if (ObjClass == nullptr)
			{
				ObjClass = Obj->GetClass();
			}
			check(ObjClass);

			if (SharedBaseClass == nullptr)
			{
				SharedBaseClass = ObjClass;
			}

			while(!ObjClass->IsChildOf(SharedBaseClass))
			{
				SharedBaseClass = SharedBaseClass->GetSuperClass();
			}
		}
		
		check(SharedBaseClass);

		Args.Add(TEXT("NumberOfObjects"), EditingObjs.Num());
		Args.Add(TEXT("ClassName"), FText::FromString(SharedBaseClass->GetName()));
		return FText::Format(LOCTEXT("ToolkitTitle_EditingMultiple", "{NumberOfObjects} {ClassName} - {ToolkitName}"), Args);
	}
}

FText FUserDefinedEditor::GetToolkitToolTipText() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();

	check( EditingObjs.Num() > 0 );

	FFormatNamedArguments Args;
	Args.Add( TEXT("ToolkitName"), GetBaseToolkitName() );

	if( EditingObjs.Num() == 1 )
	{
		const UObject* EditingObject = EditingObjs[ 0 ];
		return FAssetEditorToolkit::GetToolTipTextForObject(EditingObject);
	}
	else
	{
		UClass* SharedBaseClass = NULL;
		for( int32 x = 0; x < EditingObjs.Num(); ++x )
		{
			UObject* Obj = EditingObjs[ x ];
			check( Obj );

			UClass* ObjClass = Cast<UClass>(Obj);
			if (ObjClass == nullptr)
			{
				ObjClass = Obj->GetClass();
			}
			check( ObjClass );

			// Initialize with the class of the first object we encounter.
			if( SharedBaseClass == nullptr )
			{
				SharedBaseClass = ObjClass;
			}

			// If we've encountered an object that's not a subclass of the current best baseclass,
			// climb up a step in the class hierarchy.
			while( !ObjClass->IsChildOf( SharedBaseClass ) )
			{
				SharedBaseClass = SharedBaseClass->GetSuperClass();
			}
		}

		check(SharedBaseClass);

		Args.Add( TEXT("NumberOfObjects"), EditingObjs.Num() );
		Args.Add( TEXT("ClassName"), FText::FromString( SharedBaseClass->GetName() ) );
		return FText::Format( LOCTEXT("ToolkitTitle_EditingMultipleToolTip", "{NumberOfObjects} {ClassName} - {ToolkitName}"), Args );
	}
}

FLinearColor FUserDefinedEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor( 0.5f, 0.0f, 0.0f, 0.5f );
}

void FUserDefinedEditor::SetPropertyVisibilityDelegate(FIsPropertyVisible InVisibilityDelegate)
{
	DetailsView->SetIsPropertyVisibleDelegate(InVisibilityDelegate);
	DetailsView->ForceRefresh();
}

void FUserDefinedEditor::SetPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled InPropertyEditingDelegate)
{
	DetailsView->SetIsPropertyEditingEnabledDelegate(InPropertyEditingDelegate);
	DetailsView->ForceRefresh();
}

TSharedRef<SDockTab> FUserDefinedEditor::SpawnPropertiesTab(const FSpawnTabArgs& Args)
{
	check( Args.GetTabId() == PropertiesTabId );

	return SNew(SDockTab)
		.Label( LOCTEXT("UserDefinedDetailsTitle", "Details") )
		.TabColorScale( GetTabColorScale() )
		.OnCanCloseTab_Lambda([]() { return false; })
		[
			DetailsView.ToSharedRef()
		];
}

void FUserDefinedEditor::HandleAssetPostImport(UFactory* InFactory, UObject* InObject)
{
	if (EditingObjects.Contains(InObject))
	{
		DetailsView->SetObjects(EditingObjects);
	}
}

void FUserDefinedEditor::OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap)
{
	bool bChangedAny = false;
	
	for (int32 i = 0; i < EditingObjects.Num(); i++)
	{
		UObject* SourceObject = EditingObjects[i];
		UObject* ReplacedObject = ReplacementMap.FindRef(SourceObject);

		if (ReplacedObject && ReplacedObject != SourceObject)
		{
			EditingObjects[i] = ReplacedObject;
			bChangedAny = true;
		}
	}

	if (bChangedAny)
	{
		DetailsView->SetObjects(EditingObjects);
	}
}

FString FUserDefinedEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "User Defined Asset ").ToString();
}

TSharedRef<FUserDefinedEditor> FUserDefinedEditor::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit, FGetDetailsViewObjects GetDetailsViewObjects)
{
	TSharedRef<FUserDefinedEditor> NewEditor(new FUserDefinedEditor());

	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add( ObjectToEdit );
	NewEditor->InitEditor( Mode, InitToolkitHost, ObjectsToEdit, GetDetailsViewObjects );

	return NewEditor;
}

TSharedRef<FUserDefinedEditor> FUserDefinedEditor::CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*> ObjectsToEdit, FGetDetailsViewObjects GetDetailsViewObjects)
{
	TSharedRef<FUserDefinedEditor> NewEditor(new FUserDefinedEditor());
	NewEditor->InitEditor( Mode, InitToolkitHost, ObjectsToEdit, GetDetailsViewObjects );
	return NewEditor;
}

void FUserDefinedEditor::PostRegenerateMenusAndToolbars()
{
	TArray<UClass*> ClassList;
	for (UObject* Obj : EditingObjects)
	{
		check(Obj);
		ClassList.Add(Obj->GetClass());
	}

	UClass* CommonDenominatorClass = UClass::FindCommonBase(ClassList);
	const bool bNotAllSame = (EditingObjects.Num() > 0) && (EditingObjects[0]->GetClass() != CommonDenominatorClass);

	if (CommonDenominatorClass)
	{
		TWeakObjectPtr<UClass> WeakClassPtr(CommonDenominatorClass);
		auto OnNavigateToClassCode = [WeakClassPtr]()
		{
			if (UClass* StrongClassPtr = WeakClassPtr.Get())
			{
				if (FSourceCodeNavigation::CanNavigateToClass(StrongClassPtr))
				{
					FSourceCodeNavigation::NavigateToClass(StrongClassPtr);
				}
			}
		};

		TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
				.ShadowOffset(FVector2D::UnitVector)
				.Text(bNotAllSame ? LOCTEXT("SimpleAssetEditor_AssetType_Varied", "Common Asset Type: ") : LOCTEXT("SimpleAssetEditor_AssetType", "Asset Type: "))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 8.0f, 0.0f)
			[
				SNew(SHyperlink)
				.Style(FEditorStyle::Get(), "Common.GotoNativeCodeHyperlink")
				.OnNavigate_Lambda(OnNavigateToClassCode)
				.Text(FText::FromName(CommonDenominatorClass->GetFName()))
				.ToolTipText(FText::Format(LOCTEXT("GoToCode_ToolTip", "Click to open this source file in {0}"), FSourceCodeNavigation::GetSelectedSourceCodeIDE()))
			];
	
		SetMenuOverlay(MenuOverlayBox);
	}
}

#undef LOCTEXT_NAMESPACE
