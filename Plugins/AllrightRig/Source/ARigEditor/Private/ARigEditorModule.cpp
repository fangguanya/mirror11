#include "ARigEditorPrivatePCH.h"
#include "ARigMenu.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"

#include "TutorialMetaData.h"
#include "SDockTab.h"

#define LOCTEXT_NAMESPACE "FARigEditorModule"

void FARigEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("ARigMenu", FOnGetDetailCustomizationInstance::CreateStatic(&FARigDetails::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		struct Local
		{
			static void AddMenuCommands(FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.AddSubMenu(LOCTEXT("AllrightTools", "Allright Tools"),
					LOCTEXT("AllrightToolsTooltip", "List of tools"),
					FNewMenuDelegate::CreateStatic(&FARigEditorModule::CreateToolListMenu));
			}
		};
		// add menu item
		TSharedRef<FExtender> MenuExtender(new FExtender());
		MenuExtender->AddMenuExtension(
			"LevelEditor",
			EExtensionHook::After,
			NULL,
			FMenuExtensionDelegate::CreateStatic(&Local::AddMenuCommands));
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}

void FARigEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout("ARigMenu");
}

void FARigEditorModule::CreateToolListMenu(class  FMenuBuilder& MenuBuilder)
{
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* Class = *ClassIt;
		if (!Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			if (Class->IsChildOf(UARigMenu::StaticClass()))
			{
				FString FriendlyName = Class->GetName();
				FText MenuDestruction = FText::Format(LOCTEXT("Tool menu description", "{0}"), FText::FromString(FriendlyName));
				FText MenuTooltip = FText::Format(LOCTEXT("Tool menu tooltip", "Open {0}"), FText::FromString(FriendlyName));
				FUIAction Action(FExecuteAction::CreateStatic(&FARigEditorModule::TriggerTool, Class));
				MenuBuilder.AddMenuEntry(MenuDestruction, MenuTooltip, FSlateIcon(), Action);
			}
		}
	}
}

void FARigEditorModule::TriggerTool(UClass* ToolClass)
{
	UARigMenu* ARigMenu = NewObject<UARigMenu>(GetTransientPackage(), ToolClass);
	ARigMenu->AddToRoot();
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TArray<UObject*> Objects;
	Objects.Add(ARigMenu);
	TSharedRef<SWindow> Window = PropertyModule.CreateFloatingDetailsView(Objects, false);
	Window->SetTitle(FText::FromString(ToolClass->GetName()));
	Window->SetOnWindowClosed(FOnWindowClosed::CreateStatic(&FARigEditorModule::OnToolWindowClosed, ARigMenu));
	ARigMenu->OnToolTriggered();
}

void FARigEditorModule::OnToolWindowClosed(const TSharedRef<SWindow>& Window, UARigMenu* Instance)
{
	Instance->RemoveFromRoot();
	Instance->OnToolClosed();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FARigEditorModule, ARigEditor)
