#include "ARigRuntimePrivatePCH.h"
#include "AOrientConstraint.h"


AOrientConstraint::AOrientConstraint(const FObjectInitializer& ObjectInitializer)
{
	SetActorHiddenInGame(true);
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardComponent0"));
	RootComponent = BillboardComponent;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/AllrightRig/Constraints/OrientConstraintMtl"));
	BillboardComponent->AddElement(Material.Object, nullptr, true, 0.1f, 0.1f, nullptr);
	PrimaryActorTick.bCanEverTick = true;
}

void AOrientConstraint::InitializeConstraint()
{
	if (TargetActors.Num() > 1 && TargetActors.Num() < 7)
	{
		FRotator rRest = TargetActors.Last()->GetActorRotation();
		RestPosition = FVector(rRest.Roll, rRest.Pitch, rRest.Yaw);
		TArray <FRotator> Rotators;
		TArray <float> Weights;
		for (int32 i = 0; i < (TargetActors.Num() - 1); i ++)
		{
			Rotators.Add(TargetActors[i]->GetActorRotation());
			Weights.Add(1.0f);
		}
		FRotator Rotator = UARigFunctions::GetWeightedAverageRotator(Rotators, Weights);
		FRotator rOffset = FRotator(Rotator.Quaternion().Inverse() * rRest.Quaternion());
		Offset = FVector(rOffset.Roll, rOffset.Pitch, rOffset.Yaw);
		InitWeights();
		bEnable = true;
	}
}

void AOrientConstraint::AddTarget(AActor * TargetActor)
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

void AOrientConstraint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bEnable && TargetActors.Num())
	{
		TArray <float> Weights;
		TickWeights(Weights);
		TArray <FRotator> Rotators;
		float WeightsSum = 0.0f;
		for (int32 i = 0; i < Weights.Num(); i ++)
		{
			Rotators.Add(TargetActors[i]->GetActorRotation());
			WeightsSum += Weights[i];
		}
		FRotator rOut = FRotator(RestPosition.Y, RestPosition.Z, RestPosition.X);
		if(WeightsSum > KINDA_SMALL_NUMBER) rOut = UARigFunctions::GetWeightedAverageRotator(Rotators, Weights);
		FRotator rOffset = FRotator(Offset.Y, Offset.Z, Offset.X);
		TargetActors.Last()->SetActorRotation(rOut.Quaternion() * rOffset.Quaternion());
	}
}
