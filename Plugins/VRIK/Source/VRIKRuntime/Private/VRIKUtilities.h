#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : VRIKUtilities
// !< Date : 5/22/2017 11:50:32 AM
// !< Author : fanggang

#include "VRIKUtilities.generated.h"

class UIKVirtualBone;

UCLASS()
class UVRIKUtilitiesFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	static void SwingRotation(TArray<UIKVirtualBone*> Bones, int32 Index, FVector SwingTarget, float Weight = 1f);
	static float PreSolve(TArray<UIKVirtualBone*> Bones);
	static void RotateAroundPoint(TArray<UIKVirtualBone*> Bones, int32 Index, FVector Point, FQuat Quat);
	static void RotateBy(TArray<UIKVirtualBone*> Bones, int32 Index, FQuat Quat);
	static void RotateBy(TArray<UIKVirtualBone*> Bones, FQuat Quat);
	static void RotateTo(TArray<UIKVirtualBone*> Bones, Int32 Index, FQuat Quat);
	static void SolveTrigonometric(TArray<UIKVirtualBone*> Bones, int32 First, int32 Second, int32 Third, FVector TargetPosition, FVector BendNormal, float Weight);
	static FVector GetDirectionToBendPoint(FVector Direction, float DirectionMag, FVector BendDirection, float SqrMag1, float SqrMag2);
	static void SolveFABRIK(TArray<UIKVirtualBone*> Bones, FVector StartPosition, FVector TargetPosition, float Weight, float MinNormalizedTargetDistance, int32 Iterations, float Length);
	static FVector SolveFABRIKJoint(FVector Pos1, FVector Pos2, float Length);
	static void SolveCCD(TArray<UIKVirtualBone*> Bones, FVector TargetPosition, float Weight, int32 Iterations);

public:
	static FQuat FromToAroundAxis(FVector From, FVector To, FVector Axis);
	static FQuat RotationToLocalSpace(FQuat Space, FQuat Quat);
	static FQuat FromToRotation(FQuat From, FQuat To);
	static FVector GetAxis(FVector V);
	static FQuat ClampRotation(FQuat Quat, float ClampWeight, int32 ClampSmoothing);
};