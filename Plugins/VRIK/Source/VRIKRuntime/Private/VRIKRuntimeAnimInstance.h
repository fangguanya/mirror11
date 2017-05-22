#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : VRIKRuntimeAnimInstance
// !< Date : 5/22/2017 10:27:25 AM
// !< Author : fanggang

#include "Animation/AnimInstance.h"
#include "VRIKRuntimeAnimInstance.generated.h"

USTRUCT()
struct FIKReference
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Pelvis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Spine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Chest;					// Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Neck;					// Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftShoulder;			// Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftUpperArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftForeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightShoulder;			// Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightUpperArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightForeArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftThigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftCalf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference LeftToes;				// Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightThigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightCalf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRIK)
		FBoneReference RightToes;				// Optional
};

UCLASS(transient)
class UVRIKRuntimeAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = VRIK)
		FIKReference Reference;
};