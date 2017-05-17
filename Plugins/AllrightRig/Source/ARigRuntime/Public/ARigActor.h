#pragma once

#include "ARigStructures.h"
#include "Animation/SkeletalMeshActor.h"
#include "ARigActor.generated.h"

/*RigActor is a tickable actor based on SkeletalMeshActor*/
UCLASS()
class ARIGRUNTIME_API ARigActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		if (bTickInEditor) EditorTick();
	};
	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
	/*Tick in editor event*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void EditorTick();
	/*Create rig event*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void CreateRig();
	/*Delete rig event*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void DeleteRig();
	/*Bake current animation to the rig*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void BakeToRig(ULevelSequence *LevelSequence, UAnimSequence *AnimToBake, int32 AnimOffset, int32 StartFrame, int32 EndFrame);
	/*Bake weighted animation from sequencer to the rig*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void BakeFromSequencer(ULevelSequence *LevelSequence, int32 StartFrame, int32 EndFrame);
	/*Bake current animation from rig to an animation asset*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void BakeToAnim(ULevelSequence *LevelSequence, int32 StartFrame, int32 EndFrame, bool bUpdateSource);
	/*Snap all fk and ik controllers to current bone positions*/
	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = "true"), Category = "RigActor")
		void SnapFkIk();
	/*Enable editor tick*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		bool bTickInEditor;
	/*Source actor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		ASkeletalMeshActor * SourceActor;
	/*Rig create settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FCreateSettings CreateSettings;
	/*Rig default settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FUpdateSettings UpdateSettings;
	/*Face rig settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FFaceSettings FaceSettings;
	/*Custom controllers settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FCustomController> CustomControllers;
	/*Custom controllers settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FCustomControllersNames CustomControllersNames;
	/*Bone names and transforms array*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FBonesTransfroms> BonesTransfroms;
	/*Sequencers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <ULevelSequence*> Sequencers;

	/*RigActor constructor*/
	ARigActor(const FObjectInitializer& ObjectInitializer)
	{
		PrimaryActorTick.bCanEverTick = true;
		bTickInEditor = false;
		GetSkeletalMeshComponent()->bSelectable = false;
		UpdateSettings.bFingers = true;
		CreateSettings.RigBaseBones =
		{
			FName("root"),
			FName("pelvis"),
			FName("spine_01"),
			FName("spine_02"),
			FName("spine_03"),
			FName("neck_01"),
			FName("head"),
			FName("clavicle_l"),
			FName("upperarm_l"),
			FName("lowerarm_l"),
			FName("hand_l"),
			FName("clavicle_r"),
			FName("upperarm_r"),
			FName("lowerarm_r"),
			FName("hand_r"),
			FName("thigh_l"),
			FName("calf_l"),
			FName("foot_l"),
			FName("thigh_r"),
			FName("calf_r"),
			FName("foot_r"),
		};
		CreateSettings.RigExtraBones =
		{
			FName("thumb_01_l"),
			FName("thumb_02_l"),
			FName("thumb_03_l"),
			FName("index_01_l"),
			FName("index_02_l"),
			FName("index_03_l"),
			FName("middle_01_l"),
			FName("middle_02_l"),
			FName("middle_03_l"),
			FName("ring_01_l"),
			FName("ring_02_l"),
			FName("ring_03_l"),
			FName("pinky_01_l"),
			FName("pinky_02_l"),
			FName("pinky_03_l"),
			FName("thumb_01_r"),
			FName("thumb_02_r"),
			FName("thumb_03_r"),
			FName("index_01_r"),
			FName("index_02_r"),
			FName("index_03_r"),
			FName("middle_01_r"),
			FName("middle_02_r"),
			FName("middle_03_r"),
			FName("ring_01_r"),
			FName("ring_02_r"),
			FName("ring_03_r"),
			FName("pinky_01_r"),
			FName("pinky_02_r"),
			FName("pinky_03_r"),
			FName("ball_l"),
			FName("ball_r"),

		};
		CreateSettings.bPoleVectorSimple = false;
		CreateSettings.MidColor = FLinearColor(1.f, 1.f, 0.f);
		CreateSettings.LeftColor = FLinearColor(0.f, 0.f, 1.f);
		CreateSettings.RightColor = FLinearColor(1.f, 0.f, 0.f);
		CreateSettings.MainScale = 5.f;
		CreateSettings.HipsScale = 2.f;
		CreateSettings.FkScale = 0.5f;
		CreateSettings.IkScale = 0.5f;
		CreateSettings.FingersScale = 0.1f;
		FaceSettings.bFace = false;
		FaceSettings.EyelidsSettings.Transforms =
		{
			FTransform(FRotator(0, 0, 0), FVector(2.5, 0, 3), FVector(1, 1, 1)),
			FTransform(FRotator(0, 0, 0), FVector(-2.5, 0, 3), FVector(1, 1, 1)),
			FTransform(FRotator(0, 0, 0), FVector(2.5, 0, 1), FVector(1 ,1, 1)),
			FTransform(FRotator(0, 0, 0), FVector(-2.5, 0, 1), FVector(1, 1, 1))
		};
	};
};