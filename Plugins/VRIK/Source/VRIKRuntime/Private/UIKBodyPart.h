#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : UIKBodyPart
// !< Date : 5/22/2017 11:37:29 AM
// !< Author : fanggang

#include "UIKBodyPart.generated.h"

class UIKVirtualBone;

UCLASS()
class UIKBodyPart : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PreSolve();
	virtual void Write(TArray<FVector> RefPositions, TArray<FQuat> RefQuads);
	virtual void ApplyOffsets();
	virtual void ResetOffsets();

protected:
	virtual void OnRead(TArray<FVector> InPositions, TArray<FQuat> InQuads, bool HasChest, bool HasNeck, bool HasShoulders, bool HasToes, int32 RootIndex, int32 Index);

public:
	UPROPERTY(EditAnywhere, Category = VRIK)
		float SquaredMagnitude;
	UPROPERTY(EditAnywhere, Category = VRIK)
		float Magnitude;
	UPROPERTY()
		TArray<UIKVirtualBone*> Bones;
	UPROPERTY(EditAnywhere, Category = VRIK)
		bool Initialized;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FVector RootPosition;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FQuat RootQuat;
	UPROPERTY(EditAnywhere, Category = VRIK)
		int32 Index;

public:
	void Read(TArray<FVector> InPositions, TArray<FQuat> InQuads, bool HasChest, bool HasNeck, bool HasShoulders, bool HasToes, int32 RootIndex, int32 Index);
	void MovePosition(FVector InPosition);
	void MoveRotation(FQuat InQuat);
	void Translate(FVector InPosition, FQuat InRotation);
	void TranslateRoot(FVector InRootPos, FQuat InRootRot);
	void RotateTo(UIKVirtualBone* InBone, FQuat InQuat, float Weight = 1f);
};