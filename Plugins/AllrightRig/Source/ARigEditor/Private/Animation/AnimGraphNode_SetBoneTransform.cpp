
#include "ARigEditorPrivatePCH.h"
#include "AnimGraphNode_SetBoneTransform.h"

#define LOCTEXT_NAMESPACE "AnimGraphNode_SetBoneTransform"

UAnimGraphNode_SetBoneTransform::UAnimGraphNode_SetBoneTransform(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_SetBoneTransform::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(" Set Bone Transform");
}

FText UAnimGraphNode_SetBoneTransform::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_ModifyBone_Tooltip", "Alters translation and rotation of the bone");
}

FString UAnimGraphNode_SetBoneTransform::GetNodeCategory() const
{
	return FString("AllrightNodes");
}

void UAnimGraphNode_SetBoneTransform::CreateOutputPins()
{
	const UAnimationGraphSchema* Schema = GetDefault<UAnimationGraphSchema>();
	CreatePin(EGPD_Output, Schema->PC_Struct, TEXT(""), FComponentSpacePoseLink::StaticStruct(), false, false, TEXT("Pose"));
}

#undef LOCTEXT_NAMESPACE