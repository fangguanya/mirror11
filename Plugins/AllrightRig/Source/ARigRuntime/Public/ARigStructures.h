#pragma once
#include "ARigStructures.generated.h"

/*Axis variants*/
UENUM(BlueprintType)
enum class EAxisEnum : uint8
{
	VE_XY 	UMETA(DisplayName = "XY"),
	VE_XZ	UMETA(DisplayName = "XZ"),
	VE_YX 	UMETA(DisplayName = "YX"),
	VE_YZ	UMETA(DisplayName = "YZ"),
	VE_ZX 	UMETA(DisplayName = "ZX"),
	VE_ZY	UMETA(DisplayName = "ZY")
};

/*Structure of settings for aim constraint*/
USTRUCT(BlueprintType)
struct FAimSettings
{
	GENERATED_BODY()
	/*First axis is aim, second is up*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Settings")
		EAxisEnum AxisEnum;
	/*Invert aim axis*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Settings")
		bool AimInvert;
	/*Invert up axis*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Settings")
		bool UpInvert;
	/*Rotator offset*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Settings")
		FRotator Offset;
};

/*Body create settings*/
USTRUCT(BlueprintType)
struct FCreateSettings
{
	GENERATED_BODY()

	/*Array of base bones rig names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> RigBaseBones;
	/*Array of extra bones rig names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> RigExtraBones;
	/*Use simple algorithm to find pole vector location*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		bool bPoleVectorSimple;
	/*Middle controllers color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FLinearColor MidColor;
	/*Left controllers color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FLinearColor LeftColor;
	/*Right controllers color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FLinearColor RightColor;
	/*Main controller shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class UStaticMesh *MainShape;
	/*Fk controllers shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class UStaticMesh *FkShape;
	/*Ik controllers shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class UStaticMesh *IkShape;
	/*Pole vector controllers shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class UStaticMesh *PvShape;
	/*Main controller size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float MainScale;
	/*Hips controller size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float HipsScale;
	/*Fk controllers size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float FkScale;
	/*Ik controllers size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float IkScale;
	/*Fingers controllers size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float FingersScale;
};

/*Body update settings*/
USTRUCT(BlueprintType)
struct FUpdateSettings
{
	GENERATED_BODY()

	/*Array of base bones names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> BaseBones;
	/*Array of extra bones names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> ExtraBones;
	/*Array of controllers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> Controllers;
	/*Array of controller groups*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> ControllerGroups;
	/*Array of spine offset transforms*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FTransform> SpineOffsets;
	/*Array of local bone transforms for TwoBoneIk solver*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FTransform> LocalIkBones;
	/*Array of aim settings for aim constraints*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FAimSettings> AimSettings;
	/*Create and update fingers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		bool bFingers;
};

/*Body bake settings*/
USTRUCT(BlueprintType)
struct FBakeSettings
{
	GENERATED_BODY()

	/*Sequencer for baking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class ULevelSequence * LevelSequence;
	/*Skeletal mesh for baking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		class USkeletalMeshComponent * SkeletalMeshComponent;
	/*Start bake frame*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		int32 StartFrame;
	/*End bake frame*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		int32 EndFrame;
	/*Frames per second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		int32 Fps;
};

/*Controller settings for morph targets*/
USTRUCT(BlueprintType)
struct FMorphController
{
	GENERATED_BODY()
	/*Controller name*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName Name;
	/*Local controller position in face table*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FTransform Transform;
	/*Controller color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FLinearColor Color;
	/*Controller size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float Size;
	/*Controller shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		UStaticMesh * Shape;
	/*Array of morph targets to use on:
	0 = +X, 1 = -X, 2 = +Z, 3 = -Z
	4 = +X, 5 = -X, 6 = +Z, 7 = -Z ...*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> MorphTargets;
	/*Controller (auto filled during creation)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		AActor * Controller;
};

/*Settings for custom controller*/
USTRUCT(BlueprintType)
struct FCustomController
{
	GENERATED_BODY()
	/*Bone name*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName BoneName;
	/*Controller color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FLinearColor Color;
	/*Controller size*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		float Size;
	/*Controller shape*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		UStaticMesh * Shape;
	/*Controller parent name*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName ParentController;
};

/*Settings for custom controllers array*/
USTRUCT(BlueprintType)
struct FCustomControllersNames
{
	GENERATED_BODY()
	/*Custom controllers settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> Controllers;
	/*Bone name*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> BoneNames;
};

/*Eyes settings*/
USTRUCT(BlueprintType)
struct FEyesBones
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName LeftEye;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName RightEye;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName LeftUpperEyelid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName RightUpperEyelid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName LeftLowerEyelid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FName RightLowerEyelid;
};

/*Eyes settings*/
USTRUCT(BlueprintType)
struct FEyesSettings
{
	GENERATED_BODY()
	/*Array of eye bone names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> Bones;
	/*Array of aim settings for eye aim constraints*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FAimSettings> AimSettings;
	/*Array of eye offsets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FTransform> Offsets;
	/*Array of eye controller names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> Controllers;
};

/*Eyelids settings*/
USTRUCT(BlueprintType)
struct FEyelidsSettings
{
	GENERATED_BODY()
	/*Array of eyelids bone names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FName> Bones;
	/*Eyelids controller positions*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FTransform> Transforms;
	/*Array of eyelids offsets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FTransform> Offsets;
	/*Array of eyelids controller names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> Controllers;
};

/*Face update settings*/
USTRUCT(BlueprintType)
struct FFaceSettings
{
	GENERATED_BODY()
	/*Create and update face rig*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		bool bFace;
	/*Eyes settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FEyesSettings EyesSettings;
	/*Eyelids settings*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		FEyelidsSettings EyelidsSettings;
	/*Array of controllers for morph targets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <FMorphController> MorphControllers;
	/*Array of face controller groups*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RigActor)
		TArray <AActor*> ControllerGroups;
};

/*Names and transforms arrays*/
USTRUCT(BlueprintType)
struct FBonesTransfroms
{
	GENERATED_BODY()
	/*Array of names*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonesTransfroms")
		TArray <FName> Names;
	/*Array of transforms*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonesTransfroms")
		TArray <FTransform> Transforms;
};

/*Transformation type*/
UENUM(BlueprintType)
enum class ETransformType : uint8
{
	TE_Loc 	UMETA(DisplayName = "Location"),
	TE_Rot	UMETA(DisplayName = "Rotation"),
	TE_Scale 	UMETA(DisplayName = "Scale")
};