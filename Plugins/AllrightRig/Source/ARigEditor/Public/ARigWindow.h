#pragma once

#include "ARigMenu.h"
#include "Animation/AnimInstance.h"
#include "ARigRuntime/Public/ARigActor.h"
#include "ARigRuntime/Public/ARigFunctions.h"
#include "ARigRuntime/Public/Constraint/APointConstraint.h"
#include "ARigRuntime/Public/Constraint/AOrientConstraint.h"
#include "ARigRuntime/Public/Constraint/AScaleConstraint.h"
#include "ARigRuntime/Public/Constraint/AParentConstraint.h"
#include "ARigRuntime/Public/Constraint/AAimConstraint.h"

#include "ScopedTransaction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"

#include "ARigWindow.generated.h"

/*Rig window*/
UCLASS(Blueprintable)
class UAllrightRig : public UARigMenu
{
	GENERATED_BODY()

public:
	UAllrightRig();
//Rig
	/*Create rig for selected skeletal mesh actor*/
	UFUNCTION(Exec)
		void Rig();
	/*Delete rig for selected rig controller*/
	UFUNCTION(Exec)
		void DeleteRig();
	/*AnimBlueprint class to use in rig*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		class TSubclassOf<UAnimInstance> AnimClass;
	/*Create fingers*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		bool bFingers;
	/*Body create settings*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		FCreateSettings CreateSettings;
	/*Custom controllers settings*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		TArray <FCustomController> CustomControllers;
//Face
	/*Create face rig*/
	UPROPERTY(EditAnywhere, Category = "Face")
		bool bFace;
	/*Eye bone names*/
	UPROPERTY(EditAnywhere, Category = "Face")
		FEyesBones EyesBones;
	/*Array of controllers for morph targets*/
	UPROPERTY(EditAnywhere, Category = "Face")
		TArray <FMorphController> MorphControllers;
//Bake
	/*Bake animation to the rig. 
	* Set AnimToBake, AnimationOffset, StartFrame, EndFrame. 
	* Open level sequence editor and select rig controller or skeletal mesh actor to bake to.
	* If AnimToBake = nullptr - animation will be baked from skeletal mesh actor*/
	UFUNCTION(Exec)
		void BakeToRig();
	/*Bake current sequencer skeletal animation tracks to the rig.
	* Open level sequence editor, set playback range and select skeletal mesh actor with sequencer animation.*/
	UFUNCTION(Exec)
		void BakeFromSequencer();
	/*Bake current animation from rig to an animation asset.
	* Open level sequence editor, set playback range and select rig controller.*/
	UFUNCTION(Exec)
		void BakeToAnimAsset();
	/*Anim asset to bake from for BakeToRig command*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		class UAnimSequence * AnimToBake;
	/*Anim sequence start frame offset for BakeToRig command*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		int32 AnimationOffset;
	/*Start frame for BakeToRig command*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		int32 StartFrame;
	/*End frame for BakeToRig command*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		int32 EndFrame;
	/*Delete rig after baking to animation asset*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		bool bDeleteRig;
	/*Replace source actor animation with a new one after rig deletion*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		bool bUpdateSource;
//Rig Tools
	/*Snap all fk and ik controllers to current bone positions.
	* Select any rig controller.*/
	UFUNCTION(Exec)
		void SnapFkIk();
	/*Select all rig controllers for selected rig controller*/
	UFUNCTION(Exec)
		void SelectAllControllers();
// Presets
	/*Save rig window settings to Project\Saved\SaveGames*/
	UFUNCTION(Exec)
		void SaveRigSettings();
	/*Load rig window settings from Project\Saved\SaveGames*/
	UFUNCTION(Exec)
		void LoadRigSettings();
	/*Preset name*/
	UPROPERTY(EditAnywhere, Category = "Presets")
		FString PresetName;
//Functions
	/*Create rig from selected skeletal mesh actor*/
	ARigActor * CreateRigFromActor(ASkeletalMeshActor * SourceActor);
	/*Spawn RigActor from content*/
	static AActor * SpawnActorFromContent(const TCHAR *AssetPath, FTransform Transform);
	/*Get RigActor from selected controller*/
	static ARigActor * GetRigFromSelectedController();
	/*Check eyes names*/
	static bool CheckEyesBones(FEyesBones EyesBones);
};

/*Tools window*/
UCLASS(Blueprintable)
class UAnimTools : public UARigMenu
{
	GENERATED_BODY()

public:
	UAnimTools();
//Constraints
	/*Select up to 5 target actors and actor to constrain!*/
	UFUNCTION(Exec)
		void Point();
	/*Select up to 5 target actors and actor to constrain!*/
	UFUNCTION(Exec)
		void Orient();
	/*Select up to 5 target actors and actor to constrain!*/
	UFUNCTION(Exec)
		void Scale();
	/*Select up to 5 target actors and actor to constrain!*/
	UFUNCTION(Exec)
		void Parent();
	/*Select up to 5 target actors and actor to constrain!*/
	UFUNCTION(Exec)
		void Aim();
	/*Actor to aim up axis of aim constraint.
	If UpActor is null, up axis by default will be Z+*/
	UPROPERTY(EditAnywhere, Category = "Constrain")
		TLazyObjectPtr<AActor> UpActor;
//Actors
	/*Set rig actor*/
		ARigActor *RigActor;
	/*Array of selected actors*/
		TArray <AActor *> SelectedActors;
	/*Set rotation offsets*/
		TArray <FRotator> rOffsets;
	/*Actor class to work with*/
	UPROPERTY(EditAnywhere, Category = "Actors")
		TSubclassOf <AActor> ActorClass;
	/*Hide all actors of class*/
	UPROPERTY(EditAnywhere, Category = "Actors")
		bool bHide;
	/*Select all actors of class*/
	UPROPERTY(EditAnywhere, Category = "Actors")
		bool bSelect;
	/*RotateRelative*/
	UPROPERTY(EditAnywhere, Category = "Actors")
		FRotator RotateRelative;
//CopyPaste
	/*Copy sequencer transform track animation from one actor to another.
	* Open level sequence editor, select source actor and target actor*/
	UFUNCTION(Exec)
		void CopyAnimation();
	/*Shift pasted animation*/
	UPROPERTY(EditAnywhere, Category = "CopyPaste")
		int32 ShiftAnim;
	/*Mirror pasted animation*/
	UPROPERTY(EditAnywhere, Category = "CopyPaste")
		bool bMirror;
	/*Mirror axis*/
	UPROPERTY(EditAnywhere, Category = "CopyPaste")
		TEnumAsByte <EAxis::Type> MirrorAxis;
//Mirror Curve
	/*Mirror sequencer animation curve from transform track.
	* Open level sequence editor and select actor*/
	UFUNCTION(Exec)
		void MirrorCurve();
	/*Animation curve type*/
	UPROPERTY(EditAnywhere, Category = "MirrorCurve")
		ETransformType CurveType;
	/*Curve axis*/
	UPROPERTY(EditAnywhere, Category = "MirrorCurve")
		TEnumAsByte <EAxis::Type> Axis;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnSelectionChanged(UObject* Obj);

	virtual void OnToolTriggered() override;

	virtual void OnToolClosed() override;
};

/*Save presets*/
UCLASS()
class UARigSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
//Rig
	/*AnimBlueprint class to use in rig*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		class TSubclassOf<UAnimInstance> AnimClass;
	/*Create fingers*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		bool bFingers;
	/*Body create settings*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		FCreateSettings CreateSettings;
	/*Custom controllers settings*/
	UPROPERTY(EditAnywhere, Category = "Rig")
		TArray <FCustomController> CustomControllers;
//Face
/*Create face rig*/
	UPROPERTY(EditAnywhere, Category = "Face")
		bool bFace;
	/*Eye bone names*/
	UPROPERTY(EditAnywhere, Category = "Face")
		FEyesBones EyesBones;
	/*Array of controllers for morph targets*/
	UPROPERTY(EditAnywhere, Category = "Face")
		TArray <FMorphController> MorphControllers;
//Bake
	/*Anim asset to bake from*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		class UAnimSequence * AnimToBake;
	/*Start bake frame*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		int32 StartFrame;
	/*End bake frame*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		int32 EndFrame;
	/*Delete rig after baking to animation asset*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		bool bDeleteRig;
	/*Replace source actor animation with a new one*/
	UPROPERTY(EditAnywhere, Category = "Bake")
		bool bUpdateSource;

	UARigSaveGame() {};
};