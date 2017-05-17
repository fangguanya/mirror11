
#include "ARigRuntimePrivatePCH.h"
#include "AnimNode_SetBonesTransforms.h"

FAnimNode_SetBonesTransforms::FAnimNode_SetBonesTransforms()
	: FAnimNode_Base()
	, TranslationMode(BM_IgnoreMode)
	, RotationMode(BM_IgnoreMode)
	, ScaleMode(BM_IgnoreMode)
	, TranslationSpace(BCS_ComponentSpace)
	, RotationSpace(BCS_ComponentSpace)
	, ScaleSpace(BCS_ComponentSpace)
	, BlendWeight(1.0f)
{
}

void FAnimNode_SetBonesTransforms::Initialize(const FAnimationInitializeContext & Context)
{
	ComponentPose.Initialize(Context);
}

void FAnimNode_SetBonesTransforms::Update(const FAnimationUpdateContext & Context)
{
	EvaluateGraphExposedInputs.Execute(Context);
	ComponentPose.Update(Context);
}

void FAnimNode_SetBonesTransforms::CacheBones(const FAnimationCacheBonesContext & Context)
{
	ComponentPose.CacheBones(Context);
}

void FAnimNode_SetBonesTransforms::EvaluateComponentSpace(FComponentSpacePoseContext& Output)
{
	ComponentPose.EvaluateComponentSpace(Output);
	if (BonesTransfroms.Names.Num() > 0 && BonesTransfroms.Transforms.Num() == BonesTransfroms.Names.Num())
	{
		const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

		TArray<FBoneTransform> BoneTransforms;
		TArray<int32> BoneIds;
		USkeletalMeshComponent* SkeletalMesh = Output.AnimInstanceProxy->GetSkelMeshComponent();
		for (int32 i = 0; i < BonesTransfroms.Names.Num(); i++)
		{
			int32 BoneIndex = SkeletalMesh->GetBoneIndex(BonesTransfroms.Names[i]);
			if (BoneIndex != INDEX_NONE) BoneIds.Add(BoneIndex);
		}
		TArray <int32> SortTemp = BoneIds;
		while (SortTemp.Num() > 0)
		{
			int32 MinValue, MinIndex;
			UKismetMathLibrary::MinOfIntArray(SortTemp, MinIndex, MinValue);
			int32 b = BoneIds.Find(MinValue);
			int32 BoneIndex = SkeletalMesh->GetBoneIndex(BonesTransfroms.Names[b]);
			FTransform OutTransform = Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(BoneIndex));
			if (ScaleMode != BM_IgnoreMode)
			{
				FAnimationRuntime::ConvertCSTransformToBoneSpace(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), ScaleSpace);
				if (ScaleMode == BM_AdditiveMode) OutTransform.SetScale3D(OutTransform.GetScale3D() * BonesTransfroms.Transforms[b].GetScale3D());
				else OutTransform.SetScale3D(BonesTransfroms.Transforms[b].GetScale3D());
				FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), ScaleSpace);
			}
			if (RotationMode != BM_IgnoreMode)
			{
				FAnimationRuntime::ConvertCSTransformToBoneSpace(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), RotationSpace);
				if (RotationMode == BM_AdditiveMode) OutTransform.SetRotation(BonesTransfroms.Transforms[b].GetRotation() * OutTransform.GetRotation());
				else OutTransform.SetRotation(BonesTransfroms.Transforms[b].GetRotation());
				FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), RotationSpace);
			}
			if (TranslationMode != BM_IgnoreMode)
			{
				FAnimationRuntime::ConvertCSTransformToBoneSpace(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), TranslationSpace);
				if (TranslationMode == BM_AdditiveMode) OutTransform.AddToTranslation(BonesTransfroms.Transforms[b].GetLocation());
				else OutTransform.SetTranslation(BonesTransfroms.Transforms[b].GetLocation());
				FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkeletalMesh, Output.Pose, OutTransform, FCompactPoseBoneIndex(BoneIndex), TranslationSpace);
			}
			//OutTransform.SetToRelativeTransform(SkeletalMesh->ComponentToWorld);
			BoneTransforms.Add(FBoneTransform(FCompactPoseBoneIndex(BoneIds[b]), OutTransform));
			SortTemp.RemoveAt(MinIndex);
		}
		if (BoneTransforms.Num() > 0) Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, BlendWeight);
	}
}

