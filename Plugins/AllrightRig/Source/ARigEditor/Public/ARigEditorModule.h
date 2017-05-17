#pragma once

#include "ARigMenu.h"
#include "ARigDetails.h"
#include "ModuleManager.h"

class FARigEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static void CreateToolListMenu(class FMenuBuilder& MenuBuilder);
	static void TriggerTool(UClass* ToolClass);
	static void OnToolWindowClosed(const TSharedRef<SWindow>& Window, UARigMenu* Instance);

};
