#include "ARigEditorPrivatePCH.h"
#include "ARigDetails.h"

#define LOCTEXT_NAMESPACE "FARigEditorModule"

TSharedRef<IDetailCustomization> FARigDetails::MakeInstance()
{
	return MakeShareable(new FARigDetails);
}

void FARigDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSet<UClass*> Classes;
	TArray<TWeakObjectPtr<UObject>> objectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(objectsBeingCustomized);
	for (auto WeakObject : objectsBeingCustomized)
	{
		if (UObject* Instance = WeakObject.Get())
		{
			Classes.Add(Instance->GetClass());
		}
	}
	// Add category
	IDetailCategoryBuilder& RigCategory = DetailBuilder.EditCategory("Rig");
	IDetailCategoryBuilder& BakeCategory = DetailBuilder.EditCategory("Bake");
	IDetailCategoryBuilder& ControllersCategory = DetailBuilder.EditCategory("Controllers");
	IDetailCategoryBuilder& PresetsCategory = DetailBuilder.EditCategory("Presets");
	IDetailCategoryBuilder& CopyPasteCategory = DetailBuilder.EditCategory("CopyPaste");
	IDetailCategoryBuilder& MirrorCurveCategory = DetailBuilder.EditCategory("MirrorCurve");
	IDetailCategoryBuilder& ConstrainCategory = DetailBuilder.EditCategory("Constrain");
	IDetailCategoryBuilder& ActorsCategory = DetailBuilder.EditCategory("Actors");
	// Create a button for each element
	for (UClass* Class : Classes)
	{
		for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
		{
			UFunction* Function = *FuncIt;
			if (Function->HasAnyFunctionFlags(FUNC_Exec) && (Function->NumParms == 0))
			{
				const FString functionName = Function->GetName();
				const FText buttonCaption = FText::FromString(functionName);
				const FText filteringString = FText::FromString(functionName);
				const TAttribute <FText> ToolTip = FText::Format(LOCTEXT("ToolTipText", "{0}"), Function->GetToolTipText());
				if (functionName == "Rig" || functionName == "DeleteRig")
				{
					RigCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "BakeToRig" || functionName == "BakeToAnimAsset" || functionName == "BakeFromSequencer")
				{
					BakeCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "SnapFkIk" || functionName == "SelectAllControllers")
				{
					ControllersCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "SaveRigSettings" || functionName == "LoadRigSettings")
				{
					PresetsCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "CopyAnimation")
				{
					CopyPasteCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "MirrorCurve")
				{
					MirrorCurveCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
							.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "Point" || functionName == "Orient" || functionName == "Parent" || functionName == "Scale" || functionName == "Aim")
				{
					ConstrainCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
						.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
				if (functionName == "RelativeRotationTool")
				{
					ActorsCategory.AddCustomRow(filteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(buttonCaption)
						.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FARigDetails::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
			}
		}
	}
}

FReply FARigDetails::ExecuteToolCommand(IDetailLayoutBuilder* DetailBuilder, UFunction* MethodToExecute)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder->GetObjectsBeingCustomized(ObjectsBeingCustomized);
	for (auto WeakObject : ObjectsBeingCustomized)
	{
		if (UObject* INstance = WeakObject.Get())
		{
			INstance->CallFunctionByNameWithArguments(*MethodToExecute->GetName(), *GLog, nullptr, true);
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE