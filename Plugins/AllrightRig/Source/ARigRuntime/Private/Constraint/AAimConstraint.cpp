#include "ARigRuntimePrivatePCH.h"
#include "AAimConstraint.h"


AAimConstraint::AAimConstraint(const FObjectInitializer& ObjectInitializer)
{
	SetActorHiddenInGame(true);
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardComponent0"));
	RootComponent = BillboardComponent;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/AllrightRig/Constraints/AimConstraintMtl"));
	BillboardComponent->AddElement(Material.Object, nullptr, true, 0.1f, 0.1f, nullptr);
	PrimaryActorTick.bCanEverTick = true;
	UpVector = FVector(0.0f, 0.0f, 1.0f);
}

void AAimConstraint::InitializeConstraint()
{
	if (TargetActors.Num() > 1 && TargetActors.Num() < 7)
	{
		FRotator rRest = TargetActors.Last()->GetActorRotation();
		RestPosition = FVector(rRest.Roll, rRest.Pitch, rRest.Yaw);
		TArray <FVector> Vectors;
		TArray <float> Weights;
		for (int32 i = 0; i < (TargetActors.Num() - 1); i++)
		{
			Vectors.Add(TargetActors[i]->GetActorLocation());
			Weights.Add(1.0f);
		}
		FVector vAim = UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
		if (UpActor != nullptr)
		{
			UARigFunctions::GetAimSettings(vAim, UpActor->GetActorLocation(), TargetActors.Last()->GetActorTransform(), AimSettings);
		}
		else 
		{
			FVector vUp = UpVector + TargetActors.Last()->GetActorLocation();
			UARigFunctions::GetAimSettings(vAim, vUp, TargetActors.Last()->GetActorTransform(), AimSettings);
		}
		InitWeights();
		bEnable = true;
	}
}

void AAimConstraint::AddTarget(AActor * TargetActor)
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

void AAimConstraint::Tick(float DeltaTime)
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
		FRotator rOut = FRotator(RestPosition.Y, RestPosition.Z, RestPosition.X);
		if (WeightsSum > KINDA_SMALL_NUMBER)
		{
			FVector vAim = UARigFunctions::GetWeightedAverageVector(Vectors, Weights);
			if (UpActor != nullptr)
			{
				UARigFunctions::AimConstraint(vAim, UpActor->GetActorLocation(), TargetActors.Last()->GetActorLocation(), AimSettings, rOut);
			}
			else
			{
				FVector vUp = UpVector + TargetActors.Last()->GetActorLocation();
				UARigFunctions::AimConstraint(vAim, vUp, TargetActors.Last()->GetActorLocation(), AimSettings, rOut);
			}
		}
		TargetActors.Last()->SetActorRotation(rOut);
	}
}
