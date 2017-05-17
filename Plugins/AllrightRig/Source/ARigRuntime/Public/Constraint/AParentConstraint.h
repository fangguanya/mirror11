#pragma once
#include "AConstraint.h"
#include "AParentConstraint.generated.h"

/*Parent constraint is a tickable actor*/
UCLASS()
class ARIGRUNTIME_API AParentConstraint : public AConstraint
{
	GENERATED_BODY()
public:
	/*Point constraint constructor*/
	AParentConstraint(const FObjectInitializer& ObjectInitializer);

	/*Constraint offset*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		TArray <FTransform> Offsets;
	/*Constraint offset*/
	/*Position in which constraint was created.
	Constrained actor will take this position when ConstraintWeight = 0*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		FTransform RestPosition;

	void InitializeConstraint();
	void AddTarget(AActor * TargetActor);
	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
	virtual void Tick(float DeltaTime);
	
};
