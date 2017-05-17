#pragma once

#include "Engine.h"

#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARigStructures.h"
#include "AnimationRuntime.h"

#include "AnimNode_SetBonesTransforms.generated.h"

UENUM()
enum EBoneModMode
{
	/** The modifier ignores this channel (keeps the existing bone translation, rotation, or scale). */
	BM_IgnoreMode UMETA(DisplayName = "Ignore"),
	/** The modifier replaces the existing translation, rotation, or scale. */
	BM_ReplaceMode UMETA(DisplayName = "Replace Existing"),
	/** The modifier adds to the existing translation, rotation, or scale. */
	BM_AdditiveMode UMETA(DisplayName = "Add to Existing")
};

USTRUCT()
struct ARIGRUNTIME_API FAnimNode_SetBonesTransforms : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
		FComponentSpacePoseLink ComponentPose;
	/*Names of bones and their transforms*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		FBonesTransfroms BonesTransfroms;
	/** Whether and how to modify the translation of this bone. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Translation)
		TEnumAsByte<EBoneModMode> TranslationMode;
	/** Whether and how to modify the translation of this bone. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TEnumAsByte<EBoneModMode> RotationMode;
	/** Whether and how to modify the translation of this bone. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scale)
		TEnumAsByte<EBoneModMode> ScaleMode;
	/** Reference frame to apply Translation in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Translation)
		TEnumAsByte<enum EBoneControlSpace> TranslationSpace;
	/** Reference frame to apply Rotation in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
		TEnumAsByte<enum EBoneControlSpace> RotationSpace;
	/** Reference frame to apply Scale in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scale)
		TEnumAsByte<enum EBoneControlSpace> ScaleSpace;
	/*Blend pose weight*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		float BlendWeight;

public:
	FAnimNode_SetBonesTransforms();
	virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void Update(const FAnimationUpdateContext & Context) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context) override;
	virtual void EvaluateComponentSpace(FComponentSpacePoseContext& Output) override;
};