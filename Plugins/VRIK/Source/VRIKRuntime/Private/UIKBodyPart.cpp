#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : UIKBodyPart
// !< Date : 5/22/2017 6:44:40 PM
// !< Author : fanggang

#include "UIKBodyPart.h"

UIKBodyPart::UIKBodyPart(FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UIKBodyPart::Read(TArray<FVector> InPositions, TArray<FQuat> InQuads, bool HasChest, bool HasNeck, bool HasShoulders, bool HasToes, int32 RootIndex, int32 Index);
void UIKBodyPart::MovePosition(FVector InPosition);
void UIKBodyPart::MoveRotation(FQuat InQuat);
void UIKBodyPart::Translate(FVector InPosition, FQuat InRotation);
void UIKBodyPart::TranslateRoot(FVector InRootPos, FQuat InRootRot);
void UIKBodyPart::RotateTo(UIKVirtualBone* InBone, FQuat InQuat, float Weight = 1f);

