#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : VRIKUtilities
// !< Date : 5/22/2017 11:50:44 AM
// !< Author : fanggang

#include "VRIKUtilities.h"
#include "UIKVirtualBone.h"

UVRIKUtilitiesFunctionLibrary::UVRIKUtilitiesFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UVRIKUtilitiesFunctionLibrary::SwingRotation(TArray<UIKVirtualBone*> Bones, int32 Index, FVector SwingTarget, float Weight)
{
	if (Weight <= 0f)
	{
		return;
	}
	FQuat q = FQuat::FindBetween(Bones[Index]->SolverQuat * Bones[Index]->Axis, SwingTarget - Bones[Index]->SolverPosition);
	if (Weight < 1f)
	{
		q = FQuat::Slerp(FQuat::Identity, q, Weight);
	}
	for (int32 i = Index; i < Bones.Num(); ++i)
	{
		Bones[i]->SolverQuat = r * Bones[i]->SolverQuat;
	}
}
float UVRIKUtilitiesFunctionLibrary::PreSolve(TArray<UIKVirtualBone*> Bones)
{
	float length = 0;
	for (int32 i = 0;i < Bones.Num(); ++i)
	{
		UIKVirtualBone* bone = Bones[i];
		if (i < Bones.Num() - 1)
		{
			bone->SquaredMagnitude = (Bones[i + 1]->SolverPosition - bone->SolverPosition).SizeSquared();
			bone->Length = FMath::Sqrt(bone->SquaredMagnitude);
			length += bone->Length;
			bone->Axis = bone->SolverQuat.Inverse() * (Bones[i + 1]->SolverPosition - bone->SolverPosition);
		}
		else
		{
			bone->SquaredMagnitude = 0f;
			bone->Length = 0f;
		}
	}
	return length;
}
void UVRIKUtilitiesFunctionLibrary::RotateAroundPoint(TArray<UIKVirtualBone*> Bones, int32 Index, FVector Point, FQuat Quat)
{
	for (int32 i = Index; i < Bones.Num(); ++i)
	{
		UIKVirtualBone* bone = Bones[i];
		if (bone != nullptr)
		{
			FVector dir = bone->SolverPosition - Point;
			bone->SolverPosition = Point + Quat * dir;
			bone->SolverQuat = Quat * bone->SolverQuat;
		}
	}
}
void UVRIKUtilitiesFunctionLibrary::RotateBy(TArray<UIKVirtualBone*> Bones, int32 Index, FQuat Quat)
{
	UIKVirtualBone* origin = Bones[Index];
	for (int32 i = Index; i < Bones.Num(); ++i)
	{
		UIKVirtualBone* bone = Bones[i];
		if (bone != nullptr)
		{
			FVector dir = bone->SolverPosition - origin->SolverPosition;
			bone->SolverPosition = origin->SolverPosition + Quat * dir;
			bone->SolverQuat = Quat * bone->SolverQuat;
		}
	}
}
void UVRIKUtilitiesFunctionLibrary::RotateBy(TArray<UIKVirtualBone*> Bones, FQuat Quat)
{
	UIKVirtualBone* origin = Bones[0];
	for (int32 i = 0;i < Bones.Num(); ++i)
	{
		UIKVirtualBone* bone = Bones[i];
		if (bone != nullptr)
		{
			if (i > 0)
			{
				FVector dir = bone->SolverPosition - origin->SolverPosition;
				bone->SolverPosition = origin->SolverPosition + Quat * dir;
			}
			bone->SolverQuat = Quat * bone->SolverQuat;
		}
	}
}
void UVRIKUtilitiesFunctionLibrary::RotateTo(TArray<UIKVirtualBone*> Bones, Int32 Index, FQuat Quat)
{
	UIKVirtualBone* bone = Bones[Index];
	FQuat q = FromToRotation(bone->SolverQuat, Quat);
	RotateAroundPoint(Bones, Index, bone->SolverPosition, q);
}
void UVRIKUtilitiesFunctionLibrary::SolveTrigonometric(TArray<UIKVirtualBone*> Bones, int32 First, int32 Second, int32 Third, FVector TargetPosition, FVector BendNormal, float Weight)
{
	if (Weight < 0f)
	{
		return;
	}
	UIKVirtualBone* bone1 = Bones[First];
	UIKVirtualBone* bone2 = Bones[Second];
	UIKVirtualBone* bone3 = Bones[Third];
	TargetPosition = FMath::Lerp(bone3->SolverPosition, TargetPosition, Weight);

	FVector dir = TargetPosition - bone1->SolverPosition;

	float sqrMag = dir.SizeSquared();
	if (sqrMag == 0f)
	{
		return;
	}
	float length = FMath::Sqrt(sqrMag);

	float sqrMag1 = (bone2->SolverPosition - bone1->SolverPosition).SizeSquared();
	float sqrMag2 = (bone3->SolverPosition - bone2->SolverPosition).SizeSquared();

	FVector bendDir = FVector::CrossProduct(dir, BendNormal);
	FVector toBendPoint = GetDirectionToBendPoint(dir, length, bendDir, sqrMag1, sqrMag2);
	FQuat q1 = FQuat::FindBetween(bone2->SolverPosition - bone1->SolverPosition, toBendPoint);
	if (Weight < 1f)
	{
		q1 = FQuat::Slerp(FQuat::Identity, q1, Weight);
	}

	RotateAroundPoint(Bones, First, bone1->SolverPosition, q1);

	FQuat q2 = FQuat::FindBetween(bone3->SolverPosition - bone2->SolverPosition, TargetPosition - bone2->SolverPosition);
	if (Weight < 1f)
	{
		q2 = FQuat::Slerp(FQuat::Identity, q2, Weight);
	}

	RotateAroundPoint(Bones, Second, bone2->SolverPosition, q2);
}
FVector UVRIKUtilitiesFunctionLibrary::GetDirectionToBendPoint(FVector Direction, float DirectionMag, FVector BendDirection, float SqrMag1, float SqrMag2)
{
	float x = ((DirectionMag * DirectionMag) + (SqrMag1 - SqrMag2)) / 2f / DirectionMag;
	float y = (float)FMath::Sqrt(FMath::Clamp(SqrMag1 - x * x, 0, INFINITY));
	if (Direction == FVector::ZeroVector)
	{
		return FVector::ZeroVector
	}
	return FRotationMatrix::MakeFromXZ(Direction, BendDirection).TransformPosition(FVector(0, y, x));
}
void UVRIKUtilitiesFunctionLibrary::SolveFABRIK(TArray<UIKVirtualBone*> Bones, FVector StartPosition, FVector TargetPosition, float Weight, float MinNormalizedTargetDistance, int32 Iterations, float Length)
{
	if (Weight <= 0f)
	{
		return;
	}
	if (MinNormalizedTargetDistance > 0f)
	{
		FVector targetDirection = TargetPosition - StartPosition;
		float targetLength = targetDirection.Size();
		TargetPosition = StartPosition + (targetDirection / targetLength) * FMath::Max(Length * MinNormalizedTargetDistance, targetLength);
	}

	UIKVirtualBone* first = Bones[0];
	UIKVirtualBone* last = Bones[Bones.Num() - 1];
	for (int32 iteration = 0; iteration < Iterations; ++iteration)
	{
		last->SolverPosition = FMath::Lerp(last->SolverPosition, TargetPosition, Weight);
		for (int32 i = Bones.Num() - 2; i > -1;--i)
		{
			UIKVirtualBone* curr = Bones[i];
			UIKVirtualBone* next = Bones[i + 1];
			curr->SolverPosition = SolveFABRIKJoint(curr->SolverPosition, next->SolverPosition, curr->Length);
		}
		first->SolverPosition = StartPosition;
		for (int32 i = 1;i < Bones.Num(); ++i)
		{
			UIKVirtualBone* curr = Bones[i];
			UIKVirtualBone* prev = Bones[i - 1];
			curr->SolverPosition = SolveFABRIKJoint(curr->SolverPosition, prev->SolverPosition, prev->Length);
		}
	}

	for (int32 i = 0;i < Bones.Num() - 1; ++i)
	{
		SwingRotation(Bones, i, Bones[i + 1]->SolverPosition);
	}
}
FVector UVRIKUtilitiesFunctionLibrary::SolveFABRIKJoint(FVector Pos1, FVector Pos2, float Length)
{
	return Pos2 + (Pos1 - Pos2).GetSafeNormal() * Length;
}
void UVRIKUtilitiesFunctionLibrary::SolveCCD(TArray<UIKVirtualBone*> Bones, FVector TargetPosition, float Weight, int32 Iterations)
{
	if (Weight <= 0f)
	{
		return;
	}

	UIKVirtualBone* last = Bones[Bones.Num() - 1];
	for (int32 Iteration = 0;Iteration < Iterations; ++Iteration)
	{
		for (int32 i = Bones.Num() - 2;i > -1; --i)
		{
			UIKVirtualBone* curr = Bones[i];
			FVector toLastBone = last->SolverPosition - curr->SolverPosition;
			FVector toTarget = TargetPosition - curr->SolverPosition;
			FQuat rotation = FQuat::FindBetween(toLastBone, toTarget);
			if (Weight >= 1)
			{
				RotateBy(Bones, i, rotation);
			}
			else
			{
				RotateBy(Bones, i, FQuat::Slerp(FQuat::Identity, rotation, Weight));
			}
		}
	}
}

FQuat UVRIKUtilitiesFunctionLibrary::FromToAroundAxis(FVector From, FVector To, FVector InAxis)
{
	FQuat r = FQuat::FindBetween(From, To);
	float angle = 0;
	FVector axis = FVector::ZeroVector;
	r.ToAxisAndAngle(axis, angle);
	float dot = FVector::DotProduct(axis, InAxis);
	if (dot < 0)
	{
		angle = -angle;
	}
	return FQuat(InAxis, angle);
}
FQuat UVRIKUtilitiesFunctionLibrary::RotationToLocalSpace(FQuat Space, FQuat Quat)
{
	return (Space.Inverse() * Quat).Inverse();
}
FQuat UVRIKUtilitiesFunctionLibrary::FromToRotation(FQuat From, FQuat To)
{
	if (From == To)
	{
		return FQuat::Identity;
	}
	return To * From.Inverse();
}
FVector UVRIKUtilitiesFunctionLibrary::GetAxis(FVector V)
{
	FVector result = FVector::RightVector;
	bool neg = false;
	float x = FVector::DotProduct(V, FVector::RightVector);
	float maxAbsDot = FMath::Abs(x);
	if (x < 0f)
	{
		neg = true;
	}
	float y = FVector::DotProduct(V, FVector::UpVector);
	float absDot = FMath::Abs(y);
	if (absDot > maxAbsDot)
	{
		maxAbsDot = absDot;
		result = FVector::UpVector;
		neg = y < 0f;
	}
	float z = FVector::DotProduct(V, FVector.ForwardVector);
	absDot = FMath::Abs(z);
	if (absDot > maxAbsDot)
	{
		maxAbsDot = absDot;
		result = FVector::ForwardVector;
		neg = z < 0f;
	}
	if (neg)
	{
		result = -result;
	}
	return result;
}
FQuat UVRIKUtilitiesFunctionLibrary::ClampRotation(FQuat Quat, float ClampWeight, int32 ClampSmoothing)
{
	if (ClampWeight >= 1f)
	{
		return FQuat::Identity;
	}
	if (ClampWeight <= 0f)
	{
		return Quat;
	}
	float angle = FQuat::Identity.AngularDistance(Quat);
	float dot = 1f - (angle / PI);
	float targetClampMlp = FMath::Clamp(1f - ((ClampWeight - dot) / (1f - dot)), 0f, 1f);
	float clampMlp = FMath::Clamp(dot / ClampWeight, 0f, 1f);
	for (int32 i = 0;i < ClampSmoothing; ++i)
	{
		float sinF = clampMlp * HALF_PI;
		clampMlp = FMath::Sin(sinF);
	}
	return FQuat::Slerp(FQuat::Identity, Quat, clampMlp * targetClampMlp);
}