#pragma once

#if WITH_EDITOR

#include "UnrealEd.h"

#include "AnimationEditorUtils.h"

class SCreateAnimAssetWindow : public SWindow
{
public:
	SLATE_BEGIN_ARGS(SCreateAnimAssetWindow)
	{
	}
	SLATE_ARGUMENT(FText, DefaultAssetPath)
		SLATE_END_ARGS()

		SCreateAnimAssetWindow()
		: UserResponse(EAppReturnType::Cancel)
	{
	}

	void Construct(const FArguments& InArgs);

public:
	/** Displays the dialog in a blocking fashion */
	EAppReturnType::Type ShowModal();

	/** Gets the resulting asset path */
	FString GetAssetPath();

	/** Gets the resulting asset name */
	FString GetAssetName();

	/** Gets the resulting full asset path (path+'/'+name) */
	FString GetFullAssetPath();

protected:
	void OnPathChange(const FString& NewPath);
	void OnNameChange(const FText& NewName, ETextCommit::Type CommitInfo);
	FReply OnButtonClick(EAppReturnType::Type ButtonID);

	bool ValidatePackage();

	EAppReturnType::Type UserResponse;
	FText AssetPath;
	FText AssetName;

	static FText LastUsedAssetPath;
};

#endif