#pragma once
#include "AConstraint.h"
#include "AOrientConstraint.generated.h"

/*Point constraint is a tickable actor*/
UCLASS()
class ARIGRUNTIME_API AOrientConstraint : public AConstraint
{
	GENERATED_BODY()
public:
	/*Point constraint constructor*/
	AOrientConstraint(const FObjectInitializer& ObjectInitializer);

	/*Constraint offset*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		FVector Offset;
	/*Position in which constraint was created. 
	Constrained actor will take this position when ConstraintWeight = 0*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		FVector RestPosition;

	void InitializeConstraint();
	void AddTarget(AActor * TargetActor);
	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
	virtual void Tick(float DeltaTime);
	
};
