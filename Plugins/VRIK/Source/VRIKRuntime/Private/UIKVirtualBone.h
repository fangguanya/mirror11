#pragma once
// !< All right is reserved by HW-Storm Game Studio, Even not mentioned clearly with signature.
// !< This is not a free ware, Please do-not copy it outside of HW-Storm Game Studio
// !< File : UIKVirtualBone
// !< Date : 5/22/2017 11:47:11 AM
// !< Author : fanggang

#include "UIKVirtualBone.generated.h"

UCLASS()
class UIKVirtualBone : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, Category = VRIK)
		FVector ReadPosition;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FQuat ReadQuat;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FVector SolverPosition;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FQuat SolverQuat;

	UPROPERTY(EditAnywhere, Category = VRIK)
		float Length;
	UPROPERTY(EditAnywhere, Category = VRIK)
		float SquaredMagnitude;
	UPROPERTY(EditAnywhere, Category = VRIK)
		FVector Axis;

public:
	void Read(FVector InPosition, FQuat InQuat);
};
