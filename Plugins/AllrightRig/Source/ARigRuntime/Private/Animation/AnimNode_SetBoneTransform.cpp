
#include "ARigRuntimePrivatePCH.h"
#include "AnimNode_SetBoneTransform.h"

FAnimNode_SetBoneTransform::FAnimNode_SetBoneTransform()
	: FAnimNode_Base()
	, BoneName(FName("None"))
	, Location(FVector::ZeroVector)
	, Rotation(FRotator::ZeroRotator)
	, Scale(FVector(1.0f, 1.0f, 1.0f))
	, bLocation(true)
	, bRotation(true)
	, bScale(true)
	, BlendWeight(1.0f)
{
}

void FAnimNode_SetBoneTransform::Initialize(const FAnimationInitializeContext & Context)
{
	ComponentPose.Initialize(Context);
}

void FAnimNode_SetBoneTransform::Update(const FAnimationUpdateContext & Context)
{
	ComponentPose.Update(Context);
	EvaluateGraphExposedInputs.Execute(Context);
}

void FAnimNode_SetBoneTransform::CacheBones(const FAnimationCacheBonesContext & Context)
{
	ComponentPose.CacheBones(Context);
}

void FAnimNode_SetBoneTransform::EvaluateComponentSpace(FComponentSpacePoseContext& Output)
{
	ComponentPose.EvaluateComponentSpace(Output);
	USkeletalMeshComponent* SkeletalMesh = Output.AnimInstanceProxy->GetSkelMeshComponent();
	int32 BoneIndex = SkeletalMesh->GetBoneIndex(BoneName);
	if (BoneIndex != INDEX_NONE && (bLocation || bRotation || bScale))
	{
		FTransform OutTransform = Output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(BoneIndex));
		OutTransform *= SkeletalMesh->ComponentToWorld;
		if (bLocation) OutTransform.SetTranslation(Location);
		if (bRotation) OutTransform.SetRotation(Rotation.Quaternion());
		if (bScale) OutTransform.SetScale3D(Scale);
		OutTransform.SetToRelativeTransform(SkeletalMesh->ComponentToWorld);
		TArray<FBoneTransform> BoneTransforms;
		BoneTransforms.Add(FBoneTransform(FCompactPoseBoneIndex(BoneIndex), OutTransform));
		if (BoneTransforms.Num() > 0) Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, BlendWeight);
	}
}