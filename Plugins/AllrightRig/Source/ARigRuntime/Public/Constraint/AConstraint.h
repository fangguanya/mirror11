#pragma once
#include "AConstraint.generated.h"

/*Constraint is a tickable actor*/
UCLASS(HideCategories = ("Rendering", "Input", "Actor", "ConstraintCustom"))
class ARIGRUNTIME_API AConstraint : public AActor
{
	GENERATED_BODY()
public:
	/*Show icon*/
	UPROPERTY(BlueprintReadWrite, Category = "Constraint")
		class UMaterialBillboardComponent * BillboardComponent;
	/*EditCondition for weight2*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ConstraintCustom")
		bool bW2;
	/*EditCondition for weight3*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ConstraintCustom")
		bool bW3;
	/*EditCondition for weight2*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ConstraintCustom")
		bool bW4;
	/*EditCondition for weight3*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ConstraintCustom")
		bool bW5;
	/*Target weight 1*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, meta = (ClampMin = 0), Category = "Constraint")
		float W1;
	/*Target weight 2*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, meta = (ClampMin = 0, EditCondition = bW2), Category = "Constraint")
		float W2;
	/*Target weight 3*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, meta = (ClampMin = 0, EditCondition = bW3), Category = "Constraint")
		float W3;
	/*Target weight 4*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, meta = (ClampMin = 0, EditCondition = bW4), Category = "Constraint")
		float W4;
	/*Target weight 5*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, meta = (ClampMin = 0, EditCondition = bW5), Category = "Constraint")
		float W5;
	/*Array of actors to work with.
	Last actor is constrained actor*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Constraint")
		TArray <AActor*> TargetActors;
	/*Can tick*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		bool bEnable;
	/*Initialize constraint target weights*/
	void InitWeights()
	{
		int32 TargetsNum = TargetActors.Num();
		if (TargetsNum > 1)
		{
			W1 = 1.0f;
			W2 = 0.0f;
			W3 = 0.0f;
			W4 = 0.0f;
			W5 = 0.0f;
			bW2 = false;
			bW3 = false;
			bW4 = false;
			bW5 = false;
			if (TargetsNum > 2)
			{
				bW2 = true;
				W2 = 1.0f;
			}
			if (TargetsNum > 3)
			{
				bW3 = true;
				W3 = 1.0f;
			}
			if (TargetsNum > 4)
			{
				bW4 = true;
				W4 = 1.0f;
			}
			if (TargetsNum > 5)
			{
				bW5 = true;
				W5 = 1.0f;
			}
		}
	};
	/*Tick constraint target weights*/
	void TickWeights(TArray <float> &Weights)
	{
		for (int32 i = 0; i < TargetActors.Num(); i++)
		{
			if (!TargetActors[i]->IsValidLowLevel())
			{
				TargetActors.RemoveAt(i);
				InitWeights();
			}
		}
		int32 TargetsNum = TargetActors.Num();
		if (TargetsNum > 1) Weights.Add(W1);
		if (TargetsNum > 2) Weights.Add(W2);
		if (TargetsNum > 3) Weights.Add(W3);
		if (TargetsNum > 4) Weights.Add(W4);
		if (TargetsNum > 5) Weights.Add(W5);
	};
};
