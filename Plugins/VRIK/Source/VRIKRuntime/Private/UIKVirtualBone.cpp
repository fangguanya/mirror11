#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : UIKVirtualBone
// !< Date : 5/22/2017 12:00:54 PM
// !< Author : fanggang

#include "UIKVirtualBone.h"

UIKVirtualBone::UIKVirtualBone(FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Length = 0f;
	SquaredMagnitude = 0f;
}

void UIKVirtualBone::Read(FVector InPosition, FQuat InQuat)
{
	ReadPosition = InPosition;
	ReadQuat = InQuat;
	SolverPosition = InPosition;
	SolverQuat = InQuat;
}