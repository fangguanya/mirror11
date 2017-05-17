#pragma once
#include "ATickableActor.generated.h"

/*Constraint is a tickable actor*/
UCLASS(HideCategories = ("Rendering", "Input", "Actor"))
class ARIGRUNTIME_API ATickableActor : public AActor
{
	GENERATED_BODY()
public:

	/*Show icon*/
	UPROPERTY(BlueprintReadWrite, Category = "TickableActor")
		class UBillboardComponent * BillboardComponent;
	/*Enable editor tick*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TickableActor")
		bool bTickInEditor;
	/*Tick in editor event*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "TickableActor")
		void EditorTick();

	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
	
	virtual void Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		if (bTickInEditor) EditorTick();
	};

	ATickableActor(const FObjectInitializer& ObjectInitializer)
	{
		BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent0"));
		RootComponent = BillboardComponent;
		PrimaryActorTick.bCanEverTick = true;
		bTickInEditor = false;
	};
};
