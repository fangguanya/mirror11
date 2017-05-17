
#include "ARigEditorPrivatePCH.h"
#include "AnimationGraphSchema.h"
#include "AnimGraphNode_SetBonesTransforms.h"

#define LOCTEXT_NAMESPACE "AnimGraphNode_SetBonesTransforms"

UAnimGraphNode_SetBonesTransforms::UAnimGraphNode_SetBonesTransforms(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_SetBonesTransforms::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(" Set Bones Transforms");
}

FText UAnimGraphNode_SetBonesTransforms::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_ModifyBone_Tooltip", "Alters translation and rotation of the bones array");
}

FString UAnimGraphNode_SetBonesTransforms::GetNodeCategory() const
{
	return FString("AllrightNodes");
}

void UAnimGraphNode_SetBonesTransforms::CreateOutputPins()
{
	const UAnimationGraphSchema* Schema = GetDefault<UAnimationGraphSchema>();
	CreatePin(EGPD_Output, Schema->PC_Struct, TEXT(""), FComponentSpacePoseLink::StaticStruct(), false, false, TEXT("Pose"));
}
#undef LOCTEXT_NAMESPACE