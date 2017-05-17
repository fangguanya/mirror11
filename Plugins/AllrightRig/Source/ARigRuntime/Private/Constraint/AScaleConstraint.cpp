#include "ARigRuntimePrivatePCH.h"
#include "AScaleConstraint.h"


AScaleConstraint::AScaleConstraint(const FObjectInitializer& ObjectInitializer)
{
	SetActorHiddenInGame(true);
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardComponent0"));
	RootComponent = BillboardComponent;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/AllrightRig/Constraints/ScaleConstraintMtl"));
	BillboardComponent->AddElement(Material.Object, nullptr, true, 0.1f, 0.1f, nullptr);
	PrimaryActorTick.bCanEverTick = true;
}

void AScaleConstraint::InitializeConstraint()
{
	if (TargetActors.Num() > 1 && TargetActors.Num() < 7)
	{
		RestPosition = TargetActors.Last()->GetActorScale3D();
		TArray <FVector> Vectors;
		TArray <float> Weights;
		for (int32 i = 0; i < (TargetActors.Num() - 1); i++)
		{
			Vectors.Add(TargetActors[i]->GetActorScale3D());
			Weights.Add(1.0f);
		}
		FVector Vector = UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
		Offset = RestPosition - Vector;
		InitWeights();
		bEnable = true;
	}
}

void AScaleConstraint::AddTarget(AActor * TargetActor)
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

void AScaleConstraint::Tick(float DeltaTime)
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
			Vectors.Add(TargetActors[i]->GetActorScale3D());
			WeightsSum += Weights[i];
		}
		FVector OutV = RestPosition;
		if (WeightsSum > KINDA_SMALL_NUMBER) OutV = UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
		TargetActors.Last()->SetActorScale3D(OutV + Offset);
	}
}
