#include "VRIKEditorPrivatePCH.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"

#include "TutorialMetaData.h"
#include "SDockTab.h"

#define LOCTEXT_NAMESPACE "VRIKEditorModule"

void FVRIKEditorModule::StartupModule()
{
}

void FVRIKEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVRIKEditorModule, VRIKEditor)
