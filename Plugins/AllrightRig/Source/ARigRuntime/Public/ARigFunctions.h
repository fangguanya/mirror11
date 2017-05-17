#pragma once
#include "Engine.h"
#include "ARigStructures.h"

// Sequencer
#include "LevelSequence.h"
#include "MovieScene.h"
#include "MovieSceneCommonHelpers.h"
#include "MovieScene3DTransformSection.h"
#include "MovieScene3DTransformTrack.h"
#include "MovieSceneSkeletalAnimationSection.h"
#include "MovieSceneSkeletalAnimationTrack.h"
#include "MovieSceneFloatSection.h"
#include "MovieSceneFloatTrack.h"

#include "Runtime/AnimGraphRuntime/Public/AnimSequencerInstance.h"

#if WITH_EDITOR

#include "EngineUtils.h"
#include "Editor.h"
#include "UnrealEd.h"

//Assets
#include "AssetEditorManager.h"
#include "AssetRegistryModule.h"
#include "ARigAnimAssetWindow.h"
#include "Animation/AnimSequence.h"

// Notifications
#include "NotificationManager.h"
#include "SNotificationList.h"
#include "Editor/PackagesDialog/Public/PackagesDialog.h"

#endif

#include "ARigFunctions.generated.h"

/*Library of blueprint nodes for creation of character rigs.*/
UCLASS()
class ARIGRUNTIME_API UARigFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	/////////////////////////////////////////////////////// Actor  //////////////////////////////////////////////////////////////////////////
	/*Is a stripped-down version of AttachActorToActor node which attaches one actor to another*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Parent", Keywords = "Parent Attach Actor"), Category = "AllrightNodes|Actor")
		static void Parent(AActor *Child, AActor *Parent);
	/*Rename actor in editor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RenameActor", Keywords = "Rename Actor"), Category = "AllrightNodes|Actor")
		static void RenameActor(AActor *Actor, FName NewName);
	/*Select actor in editor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SelectActorInEditor", Keywords = "Select Actor In Editor"), Category = "AllrightNodes|Actor")
		static void SelectActorInEditor(AActor *Actor, bool IsSelected);
	/*Evaluate PostEditChange*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PostEditChange", Keywords = "Post Edit Change"), Category = "AllrightNodes|Actor")
		static void PostEditChange(AActor *Actor);
	/*Set component selectable*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSelectable", Keywords = "Set Selectable"), Category = "AllrightNodes|Actor")
		static void SetSelectable(UPrimitiveComponent *Component, bool bSelectable);

	/////////////////////////////////////////////////////// Name ///////////////////////////////////////////////////////////////////////////////
	/*Appends to name*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "AppendName", Keywords = "Append To Name"), Category = "AllrightNodes|Name")
		static FName AppendName(FName A, FName B);
	/*Checks if a name matches other name*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "MatchesWildcard", Keywords = "Matches Find"), Category = "AllrightNodes|Name")
		static bool MatchesWildcard(const FName SourceName, const FName Wildcard, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase);

	/////////////////////////////////////////////////////// Sequencer ///////////////////////////////////////////////////////////////////////////////
	/*Checks if a sequencer has binding for an actor*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsSequencerHasBindingForActor", Keywords = "Is Sequencer Has Binding For Actor"), Category = "AllrightNodes|Sequencer")
		static void IsSequencerHasBindingForActor(AActor *Actor, ULevelSequence *LevelSequence, FMovieSceneBinding &MovieSceneBinding, bool &Result);
	/*Gets an array of tracks from an actors binding*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetTracksFromBinding", Keywords = "Get Tracks From Binding"), Category = "AllrightNodes|Sequencer")
		static void GetTracksFromBinding(FMovieSceneBinding MovieSceneBinding, TArray<UMovieSceneTrack*> &MovieSceneTracks);
	/*Gets an array of sections from a track*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSectionsFromTrack", Keywords = "Get Sections From Track"), Category = "AllrightNodes|Sequencer")
		static void GetSectionsFromTrack(UMovieSceneTrack *MovieSceneTrack, TArray<UMovieScene3DTransformSection*> &TransformSections);
	/*Gets an array of float sections from a binding*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetFloatSectionsFromBinding", Keywords = "Get Float Sections From Binding"), Category = "AllrightNodes|Sequencer")
		static void GetFloatSectionsFromBinding(FMovieSceneBinding MovieSceneBinding, TArray<UMovieSceneFloatSection*> &FloatSections, TArray <FName> &Properties);
	/*Sets a transform key on a section*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetTransformSectionKey", Keywords = "Set Transform Section Key"), Category = "AllrightNodes|Sequencer")
		static void SetTransformSectionKey(AActor *Actor, UMovieScene3DTransformSection* TransformSection, FTransform Transform, float Time, bool Translation, bool Rotation, bool Scale, bool &Result);
	/*Sets a sections start time*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSectionStartTime", Keywords = "Set Section Start Time"), Category = "AllrightNodes|Sequencer")
		static void SetSectionStartTime(UMovieScene3DTransformSection *TransformSection, float Time);
	/*Sets a sections end time*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSectionEndTime", Keywords = "Set Section End Time"), Category = "AllrightNodes|Sequencer")
		static void SetSectionEndTime(UMovieScene3DTransformSection *TransformSection, float Time);
	/*Gets an array of sequencer components and bindings*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSequencerComponents", Keywords = "Get Sequencer Bindings Components"), Category = "AllrightNodes|Sequencer")
		static void GetSequencerComponents(ULevelSequence *LevelSequence, TArray<FMovieSceneBinding> &MovieSceneBindings, TArray<USceneComponent*> &SceneComponents);
	/*Updates the sequencer components location in a specific time*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UpdateSequncerComponent", Keywords = "Update Sequncer Component"), Category = "AllrightNodes|Sequencer")
		static void UpdateSequncerComponent(UMovieScene3DTransformSection *TransformSection, USceneComponent *SceneComponent, float Time);
	/*Adds actors binding to the sequencer*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddBindingToSequencer", Keywords = "Add Binding To Sequencer"), Category = "AllrightNodes|Sequencer")
		static void AddBindingToSequencer(ULevelSequence *LevelSequence, AActor *Actor, FGuid &Guid);
	/*Adds a transform track to the actors binding*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddTransformTrack", Keywords = "Add Transform Track"), Category = "AllrightNodes|Sequencer")
		static void AddTransformTrack(ULevelSequence *LevelSequence, FGuid Guid, float StartTime, float EndTime, UMovieScene3DTransformSection* &TransformSection);
	/*Deletes binding from sequencer*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoveBindingFromSequencer", Keywords = "Remove Binding From Sequencer"), Category = "AllrightNodes|Sequencer")
		static void RemoveBindingFromSequencer(ULevelSequence *LevelSequence, AActor *Actor, FMovieSceneBinding MovieSceneBinding);
	/*Get sequencer transform section from actor*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetTransformSectionFromActor", Keywords = "Get Transform Section From Actor"), Category = "AllrightNodes|Sequencer")
		static UMovieScene3DTransformSection * GetTransformSectionFromActor(ULevelSequence *LevelSequence, AActor *Actor);
	/*Get sequencer skeletal animation sections from actor*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAnimSectionsFromActor", Keywords = "Get Anim Sections From Actor"), Category = "AllrightNodes|Sequencer")
		static TArray <UMovieSceneSkeletalAnimationSection*> GetAnimSectionsFromActor(ULevelSequence *LevelSequence, AActor *Actor);
	/*Copy sequencer animation for actor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CopyActorAnimation", Keywords = "Copy Actor Animation"), Category = "AllrightNodes|Sequencer")
		static void CopyActorAnimation(ULevelSequence *LevelSequence, AActor *SourceActor, AActor *TargetActor, float ShiftAnim, bool bMirror, EAxis::Type MirrorAxis = EAxis::X);
	/*Mirror transform section animation*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MirrorTransformSectionAnim", Keywords = "Mirror TransformSection Anim"), Category = "AllrightNodes|Sequencer")
		static void MirrorTransformSectionAnim(UMovieScene3DTransformSection *TransformSection, EAxis::Type MirrorAxis = EAxis::X);
	/*Mirror transform section curve animtion*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MirrorTransformSectionAnim", Keywords = "Mirror TransformSection Anim"), Category = "AllrightNodes|Sequencer")
		static void MirrorTransformSectionCurve(UMovieScene3DTransformSection *TransformSection, ETransformType TransformType, EAxis::Type Axis = EAxis::X);
	/*Shift sequencer animation for actor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShiftTransformSectionAnim", Keywords = "Shift Transform Section Anim"), Category = "AllrightNodes|Sequencer")
		static void ShiftTransformSectionAnim(UMovieScene3DTransformSection *TransformSection, float ShiftAnim);
	/*Copy animation curve*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CopyTransformSectionCurve", Keywords = "Copy Transform Section Curve"), Category = "AllrightNodes|Sequencer")
		static void CopyTransformSectionCurve(UMovieScene3DTransformSection *SourceTransformSection, UMovieScene3DTransformSection *TargetTransformSection, ETransformType TransformType, EAxis::Type Axis = EAxis::X);
	/*Get sequencer frame rate*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSequencerFrameRate", Keywords = "Get Sequencer Frame Rate"), Category = "AllrightNodes|Sequencer")
		static int32 GetSequencerFrameRate(ULevelSequence *LevelSequence);
	/*Update sequencer float section*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UpdateFloatSection", Keywords = "Update Float Section"), Category = "AllrightNodes|Sequencer")
		static void UpdateFloatSection(UMovieSceneFloatSection * FloatSection, AActor *Actor, FName PropertyName, float Time);
	/*Checks if a sequencer has binding for an actor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UpdateSkeletalMeshSection", Keywords = "Update Skeletal MeshSection"), Category = "AllrightNodes|Sequencer")
		static void UpdateSkeletalMeshSection(ULevelSequence *LevelSequence, ASkeletalMeshActor *SkelMeshActor, float Time);
	/*Update skeletal mesh anim instance*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PreviewSetAnimPosition", Keywords = "Preview Set Anim Position"), Category = "AllrightNodes|Sequencer")
		static void PreviewSetAnimPosition(USkeletalMeshComponent* SkelMeshComp, int32 SequenceIndex, UAnimSequenceBase* InAnimSequence, float InPosition, float Weight);
	/*Get sequencer playback range*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSequencerPlaybackRange", Keywords = "Get Sequencer Playback Rangee"), Category = "AllrightNodes|Sequencer")
		static void GetSequencerPlaybackRange(ULevelSequence *LevelSequence,  float &PlaybackStart, float &PlaybackEnd);

	/////////////////////////////////////////////////////// Recorder ///////////////////////////////////////////////////////////////////////////////
	/*Adds a raw transform key to an animation sequence*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddRawKeyFromPose", Keywords = "Add Raw Key From Pose"), Category = "AllrightNodes|Recorder")
		static void AddRawKeyFromPose(USkeletalMeshComponent *SkeletalMeshComponent, UAnimSequence *AnimSequence, int32 FrameRate = 25);
	/*Creates a new animation sequence for skeletal mesh component*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateAnimSequence", Keywords = "Create Anim Sequence"), Category = "AllrightNodes|Recorder")
		static void CreateAnimSequence(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequence* &AnimSequence);
	/*Applies PostProcessSequence to AnimSequence*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PostProcessAnimSequence", Keywords = "Post Process Anim Sequence"), Category = "AllrightNodes|Recorder")
		static void PostProcessAnimSequence(UAnimSequence* AnimSequence, USkeletalMeshComponent* SkeletalMeshComponent);
	/*Gets an array of bone transformations from skinned mesh component*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSpaceBases", Keywords = "Get Space Bases"), Category = "AllrightNodes|Recorder")
		static void GetSpaceBases(USkinnedMeshComponent *SkinnedMeshComponent, TArray<FTransform> &SpaceBases);
	/*Add raw morph target key into anim sequence*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddRawMorphTargetKey", Keywords = "Add Raw Morph Target Key"), Category = "AllrightNodes|Recorder")
		static void AddRawMorphTargetKey(UAnimSequence *AnimSequence, FName TargetName, float Time, float Weight);

	/////////////////////////////////////////////////////// Editor ///////////////////////////////////////////////////////////////////////////////
	/*Shows message in the corner of screen*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "NotificationBox", Keywords = "Notification Info Print"), Category = "AllrightNodes|Editor")
		static void NotificationBox(FString String, float FadeIn = 0.1f, float Expire = 1.5, float FadeOut = 0.5);
	/*Opens editor for asset*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OpenEditorForAsset", Keywords = "Open Editor For Asset"), Category = "AllrightNodes|Editor")
		static void OpenEditorForAsset(UObject *Asset);
	/*Close all editors for asset*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CloseAllEditorsForAsset", Keywords = "Close All Editors For Asset"), Category = "AllrightNodes|Editor")
		static void CloseAllEditorsForAsset(UObject *Asset);
	/*Shows message with list and possibility to select*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PromtDialogue", Keywords = "Promt Dialogue"), Category = "AllrightNodes|Editor")
		static bool PromtDialogue(FString Title, FString Message, FString Ok, FString Cancel, TArray<UObject*> Objects);
	/*Gets an array of all objects by specified class*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAssetsByClass", Keywords = "Get Assets By Class"), Category = "AllrightNodes|Editor")
		static void GetAssetsByClass(UClass *Class, TArray<UObject*> &Objects);
	/*Get asset with opened editor*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAssetWithOpenedEditor", Keywords = "Get Asset With Opened Editor"), Category = "AllrightNodes|Editor")
		static UObject * GetAssetWithOpenedEditor(UClass *Class);
	/*Returns the given transform, converted to be relative to the given ParentTransform (old style)*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ConvertTransformToRelativeReverse", Keywords = "Convert Transform To Relative Reverse"), Category = "AllrightNodes|Editor")
		static FTransform ConvertTransformToRelativeReverse(FTransform WorldTransform, FTransform LocalTransform);
	/*Modify object*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ModifyObject", Keywords = "Modify Object"), Category = "AllrightNodes|Editor")
		static void ModifyObject(UObject *Object);

	/////////////////////////////////////////////////////// Animation ///////////////////////////////////////////////////////////////////////////////
		/*Uses the same algorithm as animation blueprint*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "TwoBoneIk", Keywords = "Two Bone Ik"), Category = "AllrightNodes|Animation")
		static void TwoBoneIk(
			FTransform EffectorPos,
			FVector PoleVector,
			FTransform InUpBone,
			FTransform MidBoneLocal,
			FTransform LowBoneLocal,
			bool bUseAimSettings,
			FAimSettings UpAimSettings,
			FAimSettings MidAimSettings,
			bool bAllowStretching,
			float StretchScale,
			FTransform &UpBone,
			FTransform &MidBone,
			FTransform &LowBone);
	/*Finds location where should be the pole vector for ik*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPoleVectorLocation", Keywords = "Get Pole Vector Location"), Category = "AllrightNodes|Animation")
		static void GetPoleVectorLocation(FVector Up, FVector Mid, FVector End, float Multiply, FVector &PvPos);
	/*Aim constraint*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "AimConstraint", Keywords = "Aim Constraint"), Category = "AllrightNodes|Animation")
		static void AimConstraint(FVector Aim, FVector Up, FVector Target, FAimSettings AimSettings, FRotator &Rotation);
	/*Remap vector using aim settings*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "RemapVectorFromAim", Keywords = "Remap Vector From Aim"), Category = "AllrightNodes|Animation")
		static FVector RemapVectorFromAim(float RollValue, FVector InVector, FAimSettings AimSettings);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetRigMapping", Keywords = "Get Rig Mapping"), Category = "AllrightNodes|Animation")
		static void GetRigMapping(USkeleton *Skeleton, TArray <FName> BaseNames, TArray <FName> &RigNames);
	/*Update SkeletalMeshComponent pose in editor*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UpdateSkeletalMeshPose", Keywords = "Update Skeletal Mesh Pose"), Category = "AllrightNodes|Animation")
		static void UpdateSkeletalMeshPose(USkeletalMeshComponent *SkeletalMeshComponent);
	/*Set morph target weight in editor (for use in anim event graph)*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMorphTarget", Keywords = "Set Morph Target"), Category = "AllrightNodes|Animation")
		static void SetMorphTarget(USkeletalMeshComponent *SkeletalMeshComponent, FName TargetName, float Weight);
	/*Get weighted average rotator from array of rotators and weights*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetWeightedAverageRotator", Keywords = "Get Weighted Average Rotator"), Category = "AllrightNodes|Animation")
		static FRotator GetWeightedAverageRotator(TArray <FRotator> Rotators, TArray <float> Weights);
	/*Get weighted average vector from array of vectors and weights*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetWeightedAverageVector", Keywords = "Get Weighted Average Vector"), Category = "AllrightNodes|Animation")
		static FVector GetWeightedAverageVector(TArray <FVector> Vectors, TArray <float> Weights);
	/*Get weighted average transform from array of transforms, weights and offsets*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetWeightedAverageTransform", Keywords = "Get Weighted Average Transform"), Category = "AllrightNodes|Animation")
		static FTransform GetWeightedAverageTransform(TArray <FTransform> Transforms, TArray <FTransform> Offsets, TArray <float> Weights);	
	/*Get closest axis*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetClosestAxis", Keywords = "Get Closest Axis"), Category = "AllrightNodes|Animation")
		static void GetClosestAxis(FVector TargetVector, FTransform Transform, FString &FirstAxis, FString &SecondAxis);
	/*Get aim axis for aim constraint*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAimAxis", Keywords = "Get Aim Axis"), Category = "AllrightNodes|Animation")
		static void GetAimAxis(FVector Aim, FVector Up, FTransform Transform, FAimSettings &AimSettings);
	/*Get aim settings for aim constraint*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAimSettings", Keywords = "Get Aim Settings"), Category = "AllrightNodes|Animation")
		static void GetAimSettings(FVector Aim, FVector Up, FTransform Transform, FAimSettings &AimSettings);
};