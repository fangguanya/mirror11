#include "ARigRuntimePrivatePCH.h"
#include "APointConstraint.h"


APointConstraint::APointConstraint(const FObjectInitializer& ObjectInitializer)
{
	SetActorHiddenInGame(true);
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardComponent0"));
	RootComponent = BillboardComponent;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/AllrightRig/Constraints/PointConstraintMtl"));
	BillboardComponent->AddElement(Material.Object, nullptr, true, 0.1f, 0.1f, nullptr);
	PrimaryActorTick.bCanEverTick = true;
}

void APointConstraint::InitializeConstraint()
{
	if (TargetActors.Num() > 1 && TargetActors.Num() < 7)
	{
		RestPosition = TargetActors.Last()->GetActorLocation();
		TArray <FVector> Vectors;
		TArray <float> Weights;
		for (int32 i = 0; i < (TargetActors.Num() - 1); i++)
		{
			Vectors.Add(TargetActors[i]->GetActorLocation());
			Weights.Add(1.0f);
		}
		Offset = RestPosition - UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
		InitWeights();
		bEnable = true;
	}
}

void APointConstraint::AddTarget(AActor * TargetActor)
{
	if (TargetActor->IsValidLowLevel())
	{
		if (TargetActors.Find(TargetActor) == INDEX_NONE)
		{
			Modify();
			bEnable = false;
			TargetActors.Insert(TargetActor, (TargetActors.Num() - 1));
			InitializeConstraint();
		}
		else UARigFunctions::NotificationBox(FString("This target actor is already in use!"));
	}
}

void APointConstraint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bEnable && TargetActors.Num())
	{
		TArray <float> Weights;
		TickWeights(Weights);
		TArray <FVector> Vectors;
		float WeightsSum = 0.0f;
		for (int32 i = 0; i < Weights.Num(); i++)
		{
			Vectors.Add(TargetActors[i]->GetActorLocation());
			WeightsSum += Weights[i];
		}
		FVector vOut = RestPosition;
		if (WeightsSum > KINDA_SMALL_NUMBER) vOut = UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
		TargetActors.Last()->SetActorLocation(vOut + Offset);
	}
}
