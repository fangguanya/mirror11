#pragma once
#include "AConstraint.h"
#include "AAimConstraint.generated.h"

/*Point constraint is a tickable actor*/
UCLASS()
class ARIGRUNTIME_API AAimConstraint : public AConstraint
{
	GENERATED_BODY()
public:
	/*Point constraint constructor*/
	AAimConstraint(const FObjectInitializer& ObjectInitializer);

	/*Aim constraint settings*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Constraint")
		FAimSettings AimSettings;
	/*Vector to aim up axis of aim constraint.
	Works if UpActor is null*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		FVector UpVector;
	/*Actor to aim up axis of aim constraint*/
	UPROPERTY(VisibleAnywhere, Category = "Constraint")
		TLazyObjectPtr<AActor> UpActor;
	/*Position in which constraint was created.
	Constrained actor will take this position when ConstraintWeight = 0*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Constraint")
		FVector RestPosition;

	void InitializeConstraint();
	void AddTarget(AActor * TargetActor);
	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
	virtual void Tick(float DeltaTime);
	
};
