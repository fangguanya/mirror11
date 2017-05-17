#include "ARigRuntimePrivatePCH.h"
#include "AParentConstraint.h"


AParentConstraint::AParentConstraint(const FObjectInitializer& ObjectInitializer)
{
	SetActorHiddenInGame(true);
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardComponent0"));
	RootComponent = BillboardComponent;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/AllrightRig/Constraints/ParentConstraintMtl"));
	BillboardComponent->AddElement(Material.Object, nullptr, true, 0.1f, 0.1f, nullptr);
	PrimaryActorTick.bCanEverTick = true;
}

void AParentConstraint::InitializeConstraint()
{
	if (TargetActors.Num() > 1 && TargetActors.Num() < 7)
	{
		RestPosition = TargetActors.Last()->GetActorTransform();
		Offsets.Empty();
		for (int32 i = 0; i < (TargetActors.Num() - 1); i++)
		{
			Offsets.Add(RestPosition.GetRelativeTransform(TargetActors[i]->GetActorTransform()));
		}
		InitWeights();
		bEnable = true;
	}
}

void AParentConstraint::AddTarget(AActor * TargetActor)
{
	if (TargetActor->IsValidLowLevel())
	{
		if (TargetActors.Find(TargetActor) == INDEX_NONE)
		{
			Modify();
			bEnable = false;
			TargetActors.Insert(TargetActor, (TargetActors.Num() - 1));
			Offsets.Add(TargetActors.Last()->GetActorTransform().GetRelativeTransform(TargetActor->GetActorTransform()));
			InitWeights();
			bEnable = true;
		}
		else UARigFunctions::NotificationBox(FString("This target actor is already in use!"));
	}
}

void AParentConstraint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bEnable && TargetActors.Num())
	{
		TArray <float> Weights;
		TickWeights(Weights);
		TArray <FTransform> Transforms;
		float WeightsSum = 0.0f;
		for (int32 i = 0; i < Weights.Num(); i++)
		{
			Transforms.Add(TargetActors[i]->GetActorTransform());
			WeightsSum += Weights[i];
		}
		FTransform OutT = RestPosition;
		if (WeightsSum > KINDA_SMALL_NUMBER)
		{
			if (Transforms.Num() != Offsets.Num()) InitializeConstraint();
			OutT = UARigFunctions::GetWeightedAverageTransform(Transforms, Offsets, Weights);
		}
		TargetActors.Last()->SetActorTransform(OutT);
	}
}
