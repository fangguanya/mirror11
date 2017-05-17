#pragma once

#include "Engine.h"

#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstanceProxy.h"

#include "AnimNode_SetBoneTransform.generated.h"


USTRUCT()
struct ARIGRUNTIME_API FAnimNode_SetBoneTransform : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
		FComponentSpacePoseLink ComponentPose;

	/*Name of bone to control*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		FName BoneName;
	/*New translation of bone to apply*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		FVector Location;
	/*New rotation of bone to apply*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		FVector Scale;
	/*Modify bone location*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		bool bLocation;
	/*Modify bone rotation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		bool bRotation;
	/*Modify bone scale*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		bool bScale;
	/*Blend pose weight*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		float BlendWeight;

public:
	FAnimNode_SetBoneTransform();
	virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void Update(const FAnimationUpdateContext & Context) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context) override;
	virtual void EvaluateComponentSpace(FComponentSpacePoseContext& Output) override;
};