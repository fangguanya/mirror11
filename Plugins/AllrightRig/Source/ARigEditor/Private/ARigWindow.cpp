#include "ARigEditorPrivatePCH.h"
#include "ARigWindow.h"
#define LOCTEXT_NAMESPACE "AnimTools"

/////////////////////////////////////////////// AllrightRig /////////////////////////////////////////////////
UAllrightRig::UAllrightRig()
{
	AnimClass = nullptr;
	bFingers = true;
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MainShapeAsset(TEXT("/AllrightRig/Controls/Shapes/Circle"));
	CreateSettings.MainShape = MainShapeAsset.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FkShapeAsset(TEXT("/AllrightRig/Controls/Shapes/Sphere"));
	CreateSettings.FkShape = FkShapeAsset.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> IkShapeAsset(TEXT("/AllrightRig/Controls/Shapes/Cube"));
	CreateSettings.IkShape = IkShapeAsset.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PvShapeAsset(TEXT("/AllrightRig/Controls/Shapes/Locator"));
	CreateSettings.PvShape = PvShapeAsset.Object;
	CreateSettings.MidColor = FLinearColor(1.f, 1.f, 0.f);
	CreateSettings.LeftColor = FLinearColor(0.f, 0.f, 1.f);
	CreateSettings.RightColor = FLinearColor(1.f, 0.f, 0.f);
	CreateSettings.MainScale = 5.f;
	CreateSettings.HipsScale = 2.f;
	CreateSettings.FkScale = 0.5f;
	CreateSettings.IkScale = 0.5f;
	CreateSettings.FingersScale = 0.1f;
	bFace = false;
	AnimToBake = nullptr;
	AnimationOffset = 0;
	StartFrame = 0;
	EndFrame = 60;
	bDeleteRig = false;
	bUpdateSource = false;
};

void UAllrightRig::Rig()
{
	if (GEditor->GetSelectedActorCount() > 0)
	{
		FSelectionIterator Sel = GEditor->GetSelectedActorIterator();
		for (Sel; Sel; ++Sel)
		{
			ASkeletalMeshActor * SelActor = Cast<ASkeletalMeshActor >(*Sel);
			if (SelActor->IsValidLowLevel()) CreateRigFromActor(SelActor);
			else UARigFunctions::NotificationBox(FString("Select skeletal mesh actor in editor viewport!"));
		}
	}
	else UARigFunctions::NotificationBox(FString("Select skeletal mesh actor in editor viewport!"));
}

void UAllrightRig::DeleteRig()
{
	ARigActor *RigActor = GetRigFromSelectedController();
	if (RigActor != nullptr)
	{
		ASkeletalMeshActor * SourceActor = RigActor->SourceActor;
		if (SourceActor->IsValidLowLevel())
		{
			GEditor->BeginTransaction(FText::FromString("Delete Rig"));
			SourceActor->Modify();
			SourceActor->SetActorTransform(RigActor->GetActorTransform());
			USkeletalMeshComponent *SourceSkelMeshComp = SourceActor->GetSkeletalMeshComponent();
			SourceSkelMeshComp->Modify();
			SourceSkelMeshComp->SetVisibility(true);
			RigActor->Modify();
			RigActor->DeleteRig();
			GEditor->SelectActor(SourceActor, true, true, true, true);
			GEditor->EndTransaction();
		}
	}
	else UARigFunctions::NotificationBox(FString("Select any rig controller in viewport!"));
}

void UAllrightRig::BakeToRig()
{
	ULevelSequence * Sequencer = Cast <ULevelSequence>(UARigFunctions::GetAssetWithOpenedEditor(ULevelSequence::StaticClass()));
	if (Sequencer->IsValidLowLevel())
	{
		if (GEditor->GetSelectedActorCount() > 0)
		{
			ARigActor * RigActor = GetRigFromSelectedController();
			if (RigActor == nullptr)
			{
				FSelectionIterator Sel = GEditor->GetSelectedActorIterator();
				for (Sel; Sel; ++Sel)
				{
					ASkeletalMeshActor * SelActor = Cast<ASkeletalMeshActor >(*Sel);
					if (SelActor->IsValidLowLevel()) RigActor = CreateRigFromActor(SelActor);
					else UARigFunctions::NotificationBox(FString("Select skeletal mesh actor or rig controller in viewport!"));
				}
			}
			if (RigActor != nullptr)
			{
				GEditor->BeginTransaction(FText::FromString("Bake To Rig"));
				RigActor->BakeToRig(Sequencer, AnimToBake, AnimationOffset, StartFrame, EndFrame);
				GEditor->EndTransaction();
			}
		}
		else UARigFunctions::NotificationBox(FString("Select skeletal mesh actor or rig controller in viewport!"));
	}
	else UARigFunctions::NotificationBox(FString("Open Level Sequence Editor to work with!"));
}

void UAllrightRig::BakeFromSequencer()
{
	ULevelSequence * Sequencer = Cast <ULevelSequence>(UARigFunctions::GetAssetWithOpenedEditor(ULevelSequence::StaticClass()));
	if (Sequencer->IsValidLowLevel())
	{
		FSelectionIterator Sel = GEditor->GetSelectedActorIterator();
		for (Sel; Sel; ++Sel)
		{
			ASkeletalMeshActor * SelActor = Cast<ASkeletalMeshActor >(*Sel);
			if (SelActor->IsValidLowLevel())
			{
				ARigActor * RigActor = CreateRigFromActor(SelActor);
				if (RigActor->IsValidLowLevel())
				{
					float fPlaybackStart, fPlaybackEnd;
					UARigFunctions::GetSequencerPlaybackRange(Sequencer, fPlaybackStart, fPlaybackEnd);
					int32 Fps = UARigFunctions::GetSequencerFrameRate(Sequencer);
					int32 iPlaybackStart = FMath::RoundToInt(fPlaybackStart * Fps);
					int32 iPlaybackEnd = FMath::RoundToInt(fPlaybackEnd * Fps);
					GEditor->BeginTransaction(FText::FromString("Bake From Sequencer"));
					RigActor->BakeFromSequencer(Sequencer, iPlaybackStart, iPlaybackEnd);
					GEditor->EndTransaction();
				}
			}
			else UARigFunctions::NotificationBox(FString("Select skeletal mesh actor in editor viewport!"));
		}
	}
	else UARigFunctions::NotificationBox(FString("Open Level Sequence Editor to work with!"));
}

void UAllrightRig::BakeToAnimAsset()
{
	ARigActor *RigActor = GetRigFromSelectedController();
	if (RigActor != nullptr)
	{
		ULevelSequence * Sequencer = Cast <ULevelSequence>(UARigFunctions::GetAssetWithOpenedEditor(ULevelSequence::StaticClass()));
		if (Sequencer->IsValidLowLevel())
		{
			float fPlaybackStart, fPlaybackEnd;
			UARigFunctions::GetSequencerPlaybackRange(Sequencer, fPlaybackStart, fPlaybackEnd);
			int32 Fps = UARigFunctions::GetSequencerFrameRate(Sequencer);
			int32 iPlaybackStart = FMath::RoundToInt(fPlaybackStart * Fps);
			int32 iPlaybackEnd = FMath::RoundToInt(fPlaybackEnd * Fps);
			GEditor->BeginTransaction(FText::FromString("Bake To Anim Asset"));
			RigActor->BakeToAnim(Sequencer, iPlaybackStart, iPlaybackEnd, bUpdateSource);
			GEditor->EndTransaction();
			if (bDeleteRig) DeleteRig();
		}
		else UARigFunctions::NotificationBox(FString("Open Level Sequence Editor to work with!"));
	}
	else UARigFunctions::NotificationBox(FString("Select any rig controller in viewport!"));
}

void UAllrightRig::SnapFkIk()
{
	ARigActor *RigActor = GetRigFromSelectedController();
	if (RigActor != nullptr)
	{
		GEditor->BeginTransaction(FText::FromString("Snap Fk Ik"));
		RigActor->SnapFkIk();
		GEditor->EndTransaction();
	}
	else UARigFunctions::NotificationBox(FString("Select any rig controller in viewport!"));
}

void UAllrightRig::SelectAllControllers()
{
	ARigActor *RigActor = GetRigFromSelectedController();
	if (RigActor != nullptr)
	{
		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsWithTag(RigActor, FName(*RigActor->GetActorLabel()), Controllers);
		for (AActor *Ctrl : Controllers)
		{
			UStaticMeshComponent *StaticMeshComp = Ctrl->FindComponentByClass<UStaticMeshComponent>();
			if (StaticMeshComp!= nullptr)
			{
				if (StaticMeshComp->GetStaticMesh() != nullptr)
					GEditor->SelectActor(Ctrl, true, true, true, true);
			}
			UMaterialBillboardComponent * MatBilComp = Ctrl->FindComponentByClass<UMaterialBillboardComponent>();
			if (MatBilComp != nullptr) GEditor->SelectActor(Ctrl, true, true, true, true);
		}
	}
	else UARigFunctions::NotificationBox(FString("Select any rig controller in viewport!"));
}

void UAllrightRig::SaveRigSettings()
{
	if (PresetName.Len())
	{
		if (UGameplayStatics::DoesSaveGameExist(PresetName, 0))
		{
			TArray <UObject*> Objects;
			if (!UARigFunctions::PromtDialogue(
				FString("Save Rig Settings"),
				FString("Preset with current name already exists! Do you want to replace it?"),
				FString("Replace"),
				FString("Cancel"),
				Objects)) return;
		}
		UARigSaveGame * ARigSaveGame = Cast<UARigSaveGame>(UGameplayStatics::CreateSaveGameObject(UARigSaveGame::StaticClass()));
		ARigSaveGame->AnimClass = AnimClass;
		ARigSaveGame->bFingers = bFingers;
		ARigSaveGame->CreateSettings = CreateSettings;
		ARigSaveGame->bFace = bFace;
		ARigSaveGame->EyesBones = EyesBones;
		ARigSaveGame->MorphControllers = MorphControllers;
		ARigSaveGame->CustomControllers = CustomControllers;
		ARigSaveGame->AnimToBake = AnimToBake;
		ARigSaveGame->StartFrame = StartFrame;
		ARigSaveGame->EndFrame = EndFrame;
		ARigSaveGame->bDeleteRig = bDeleteRig;
		ARigSaveGame->bUpdateSource = bUpdateSource;
		UGameplayStatics::SaveGameToSlot(ARigSaveGame, PresetName, 0);
	}
	else UARigFunctions::NotificationBox(FString("Set preset name!"));
}

void UAllrightRig::LoadRigSettings()
{
	if (PresetName.Len())
	{
		if (UGameplayStatics::DoesSaveGameExist(PresetName, 0))
		{
			UARigSaveGame* ARigSaveGame = Cast<UARigSaveGame>(UGameplayStatics::CreateSaveGameObject(UARigSaveGame::StaticClass()));
			ARigSaveGame = Cast<UARigSaveGame>(UGameplayStatics::LoadGameFromSlot(PresetName, 0));
			AnimClass = ARigSaveGame->AnimClass;
			bFingers = ARigSaveGame->bFingers;
			CreateSettings = ARigSaveGame->CreateSettings;
			bFace = ARigSaveGame->bFace;
			EyesBones = ARigSaveGame->EyesBones;
			MorphControllers = ARigSaveGame->MorphControllers;
			CustomControllers = ARigSaveGame->CustomControllers;
			AnimToBake = ARigSaveGame->AnimToBake;
			StartFrame = ARigSaveGame->StartFrame;
			EndFrame = ARigSaveGame->EndFrame;
			bDeleteRig = ARigSaveGame->bDeleteRig;
			bUpdateSource = ARigSaveGame->bUpdateSource;
		}
		else UARigFunctions::NotificationBox(FString("Preset doesn't exist (check Saved/SaveGames/ folder)!"));
	}
	else UARigFunctions::NotificationBox(FString("Set preset name!"));
}

ARigActor * UAllrightRig::CreateRigFromActor(ASkeletalMeshActor * SourceActor)
{
	ARigActor * RigActor = nullptr;
	if (SourceActor->IsValidLowLevel())
	{
		if (AnimClass != nullptr)
		{
			USkeletalMeshComponent *SourceSkelMeshComp = SourceActor->GetSkeletalMeshComponent();
			IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(AnimClass);
			if (AnimClassInterface && AnimClassInterface->GetTargetSkeleton() == SourceSkelMeshComp->SkeletalMesh->Skeleton)
			{
				GEditor->BeginTransaction(FText::FromString("Create rig"));
				RigActor = Cast<ARigActor>(SpawnActorFromContent(TEXT("/AllrightRig/Rig"), SourceActor->GetActorTransform()));
				if (RigActor != nullptr)
				{
					RigActor->SetActorLabel((SourceActor->GetActorLabel() + "Rig"), true);
					USkeletalMeshComponent *RigSkelMeshComp = RigActor->GetSkeletalMeshComponent();
					RigSkelMeshComp->SetSkeletalMesh(SourceSkelMeshComp->SkeletalMesh);
					RigActor->UpdateSettings.bFingers = bFingers;
					RigActor->CreateSettings = CreateSettings;
					if (bFace)
					{
						RigActor->FaceSettings.bFace = false;
						if (CheckEyesBones(EyesBones))
						{
							RigActor->FaceSettings.bFace = true;
							RigActor->FaceSettings.EyesSettings.Bones =
							{
								EyesBones.LeftEye,
								EyesBones.RightEye
							};
							RigActor->FaceSettings.EyelidsSettings.Bones =
							{
								EyesBones.LeftUpperEyelid,
								EyesBones.RightUpperEyelid,
								EyesBones.LeftLowerEyelid,
								EyesBones.RightLowerEyelid
							};
							RigActor->FaceSettings.MorphControllers = MorphControllers;
						}
						else UARigFunctions::NotificationBox(FString("Face rig ignored : set eyes bones"));
					}
					RigActor->CustomControllers = CustomControllers;
					RigActor->SourceActor = SourceActor;
					if (AnimToBake == nullptr) AnimToBake = Cast<UAnimSequence>(SourceSkelMeshComp->AnimationData.AnimToPlay);
					RigActor->CreateRig();
					RigActor->GetSkeletalMeshComponent()->SetAnimInstanceClass(AnimClass);
					SourceSkelMeshComp->Modify();
					SourceSkelMeshComp->SetVisibility(false);
				}
				GEditor->EndTransaction();
			}
			else UARigFunctions::NotificationBox(FString("AnimBlueprint is not compatible with current SkeletalMesh!"));
		}
		else UARigFunctions::NotificationBox(FString("Choose an AnimBlueprint class for current SkeletalMesh!"));
	}
	return RigActor;
}

AActor * UAllrightRig::SpawnActorFromContent(const TCHAR *AssetPath, FTransform Transform)
{
	AActor * ReturnActor = nullptr;
	UObject* RigObj = StaticLoadObject(UObject::StaticClass(), nullptr, AssetPath);
	if (RigObj != nullptr)
	{
		UBlueprint *Blueprint = Cast<UBlueprint>(RigObj);
		TSubclassOf<class UObject> SpawnClass = (UClass*)Blueprint->GeneratedClass;
		ReturnActor = GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(SpawnClass, Transform);
	}
	else UARigFunctions::NotificationBox(FString("Can't find asset in AllrightRig plugin content!"));
	return ReturnActor;
}

ARigActor * UAllrightRig::GetRigFromSelectedController()
{
	ARigActor * ReturnActor = nullptr;
	FSelectionIterator Sel = GEditor->GetSelectedActorIterator();
	for (Sel; Sel; ++Sel)
	{
		AActor * SelActor = Cast<AActor >(*Sel);
		if (SelActor->Tags.Num())
		{
			FName Tag = SelActor->Tags[0];
			if (Tag != "None")
			{
				for (TActorIterator<ARigActor> It(Sel->GetWorld(), ARigActor::StaticClass()); It; ++It)
				{
					ARigActor *RigActor = *It;
					if (RigActor->GetActorLabel() == Tag.ToString()) return RigActor;
				}
			}
		}
	}
	return ReturnActor;
}

bool UAllrightRig::CheckEyesBones(FEyesBones EyesBones)
{
	bool bResult = false;
	if 
	(	
		EyesBones.LeftEye != "None" &&
		EyesBones.RightEye != "None" &&
		EyesBones.LeftUpperEyelid != "None" &&
		EyesBones.RightUpperEyelid != "None" &&
		EyesBones.LeftLowerEyelid != "None" &&
		EyesBones.RightLowerEyelid != "None"
	) bResult = true;
	return bResult;
}

///////////////////////////////////////////////// AnimTools //////////////////////////////////////////////////
UAnimTools::UAnimTools()
{
	OnSelectionChanged(GEditor->GetSelectedActors());
	bHide = false;
	MirrorAxis = EAxis::X;
	Axis = EAxis::X;
	static ConstructorHelpers::FObjectFinder<UBlueprint> Ctrl(TEXT("/AllrightRig/Controls/Ctrl"));
	ActorClass = (UClass*)Ctrl.Object->GeneratedClass;
}

void UAnimTools::Point()
{
	TArray <AActor*> SelActors;
	GEditor->GetSelectedSet(AActor::StaticClass())->GetSelectedObjects(SelActors);
	if (SelActors.Num() > 1 && SelActors.Num() < 7)
	{
		TArray <AActor*> AttachedActors;
		SelActors.Last()->GetAttachedActors(AttachedActors);
		APointConstraint * Constraint = nullptr;
		if (AttachedActors.Num())
		{
			for (AActor *Actor : AttachedActors)
			{
				if (Cast <APointConstraint>(Actor)) Constraint = Cast <APointConstraint>(Actor);
			}
		}
		if (Constraint != nullptr)
		{

			GEditor->BeginTransaction(FText::FromString("Point Constraint"));
			for (int32 i = 0; i < (SelActors.Num() - 1); i++) Constraint->AddTarget(SelActors[i]);
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->BeginTransaction(FText::FromString("Point Constraint"));
			Constraint = GEditor->GetEditorWorldContext().World()->SpawnActor<APointConstraint>(APointConstraint::StaticClass());
			Constraint->TargetActors = SelActors;
			const FName SocketName;
			GEditor->ParentActors(SelActors.Last(), Constraint, SocketName);
			Constraint->SetActorRelativeLocation(FVector::ZeroVector);
			Constraint->InitializeConstraint();
			GEditor->EndTransaction();
		}
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(Constraint, true, true, true, true);
	}
	else UARigFunctions::NotificationBox(FString("Select up to 5 target actors and actor to constrain!"));
}

void UAnimTools::Orient()
{
	TArray <AActor*> SelActors;
	GEditor->GetSelectedSet(AActor::StaticClass())->GetSelectedObjects(SelActors);
	if (SelActors.Num() > 1 && SelActors.Num() < 7)
	{
		TArray <AActor*> AttachedActors;
		SelActors.Last()->GetAttachedActors(AttachedActors);
		AOrientConstraint * Constraint = nullptr;
		if (AttachedActors.Num())
		{
			for (AActor *Actor : AttachedActors)
			{
				if (Cast <AOrientConstraint>(Actor)) Constraint = Cast <AOrientConstraint>(Actor);
			}
		}
		if (Constraint != nullptr)
		{

			GEditor->BeginTransaction(FText::FromString("Orient Constraint"));
			for (int32 i = 0; i < (SelActors.Num() - 1); i++) Constraint->AddTarget(SelActors[i]);
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->BeginTransaction(FText::FromString("Orient Constraint"));
			Constraint = GEditor->GetEditorWorldContext().World()->SpawnActor<AOrientConstraint>(AOrientConstraint::StaticClass());
			Constraint->TargetActors = SelActors;
			const FName SocketName;
			GEditor->ParentActors(SelActors.Last(), Constraint, SocketName);
			Constraint->SetActorRelativeLocation(FVector::ZeroVector);
			Constraint->InitializeConstraint();
			GEditor->EndTransaction();
		}
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(Constraint, true, true, true, true);
	}
	else UARigFunctions::NotificationBox(FString("Select up to 5 target actors and actor to constrain!"));
}

void UAnimTools::Scale()
{
	TArray <AActor*> SelActors;
	GEditor->GetSelectedSet(AActor::StaticClass())->GetSelectedObjects(SelActors);
	if (SelActors.Num() > 1 && SelActors.Num() < 7)
	{
		TArray <AActor*> AttachedActors;
		SelActors.Last()->GetAttachedActors(AttachedActors);
		AScaleConstraint * Constraint = nullptr;
		if (AttachedActors.Num())
		{
			for (AActor *Actor : AttachedActors)
			{
				if (Cast <AScaleConstraint>(Actor)) Constraint = Cast <AScaleConstraint>(Actor);
			}
		}
		if (Constraint != nullptr)
		{

			GEditor->BeginTransaction(FText::FromString("Scale Constraint"));
			for (int32 i = 0; i < (SelActors.Num() - 1); i++) Constraint->AddTarget(SelActors[i]);
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->BeginTransaction(FText::FromString("Scale Constraint"));
			Constraint = GEditor->GetEditorWorldContext().World()->SpawnActor<AScaleConstraint>(AScaleConstraint::StaticClass());
			Constraint->TargetActors = SelActors;
			const FName SocketName;
			GEditor->ParentActors(SelActors.Last(), Constraint, SocketName);
			Constraint->SetActorRelativeLocation(FVector::ZeroVector);
			Constraint->InitializeConstraint();
			GEditor->EndTransaction();
		}
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(Constraint, true, true, true, true);
	}
	else UARigFunctions::NotificationBox(FString("Select up to 5 target actors and actor to constrain!"));
}

void UAnimTools::Parent()
{
	TArray <AActor*> SelActors;
	GEditor->GetSelectedSet(AActor::StaticClass())->GetSelectedObjects(SelActors);
	if (SelActors.Num() > 1 && SelActors.Num() < 7)
	{
		TArray <AActor*> AttachedActors;
		SelActors.Last()->GetAttachedActors(AttachedActors);
		AParentConstraint * Constraint = nullptr;
		if (AttachedActors.Num())
		{
			for (AActor *Actor : AttachedActors)
			{
				if (Cast <AParentConstraint>(Actor)) Constraint = Cast <AParentConstraint>(Actor);
			}
		}
		if (Constraint != nullptr)
		{

			GEditor->BeginTransaction(FText::FromString("Parent Constraint"));
			for (int32 i = 0; i < (SelActors.Num() - 1); i++) Constraint->AddTarget(SelActors[i]);
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->BeginTransaction(FText::FromString("Parent Constraint"));
			Constraint = GEditor->GetEditorWorldContext().World()->SpawnActor<AParentConstraint>(AParentConstraint::StaticClass());
			Constraint->TargetActors = SelActors;
			const FName SocketName;
			GEditor->ParentActors(SelActors.Last(), Constraint, SocketName);
			Constraint->SetActorRelativeLocation(FVector::ZeroVector);
			Constraint->InitializeConstraint();
			GEditor->EndTransaction();
		}
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(Constraint, true, true, true, true);
	}
	else UARigFunctions::NotificationBox(FString("Select up to 5 target actors and actor to constrain!"));
}

void UAnimTools::Aim()
{
	TArray <AActor*> SelActors;
	GEditor->GetSelectedSet(AActor::StaticClass())->GetSelectedObjects(SelActors);
	if (SelActors.Num() > 1 && SelActors.Num() < 7)
	{
		TArray <AActor*> AttachedActors;
		SelActors.Last()->GetAttachedActors(AttachedActors);
		AAimConstraint * Constraint = nullptr;
		if (AttachedActors.Num())
		{
			for (AActor *Actor : AttachedActors)
			{
				if (Cast <AAimConstraint>(Actor)) Constraint = Cast <AAimConstraint>(Actor);
			}
		}
		if (Constraint != nullptr)
		{

			GEditor->BeginTransaction(FText::FromString("Aim Constraint"));
			for (int32 i = 0; i < (SelActors.Num() - 1); i++) Constraint->AddTarget(SelActors[i]);
			GEditor->EndTransaction();
		}
		else
		{
			GEditor->BeginTransaction(FText::FromString("Aim Constraint"));
			Constraint = GEditor->GetEditorWorldContext().World()->SpawnActor<AAimConstraint>(AAimConstraint::StaticClass());
			Constraint->TargetActors = SelActors;
			Constraint->UpActor = UpActor;
			const FName SocketName;
			GEditor->ParentActors(SelActors.Last(), Constraint, SocketName);
			Constraint->SetActorRelativeLocation(FVector::ZeroVector);
			Constraint->InitializeConstraint();
			GEditor->EndTransaction();
		}
		GEditor->SelectNone(true, true, false);
		GEditor->SelectActor(Constraint, true, true, true, true);
	}
	else UARigFunctions::NotificationBox(FString("Select up to 5 target actors and actor to constrain!"));
}

void UAnimTools::CopyAnimation()
{
	if (SelectedActors.Num() == 2)
	{
		ULevelSequence * Sequencer = Cast <ULevelSequence>(UARigFunctions::GetAssetWithOpenedEditor(ULevelSequence::StaticClass()));
		if (Sequencer->IsValidLowLevel())
		{
			float ShiftAnimation = float(ShiftAnim) / float(UARigFunctions::GetSequencerFrameRate(Sequencer));
			GEditor->BeginTransaction(FText::FromString("Copy Animation"));
			UARigFunctions::CopyActorAnimation(Sequencer, SelectedActors[0], SelectedActors[1], ShiftAnimation, bMirror, MirrorAxis);
			GEditor->EndTransaction();
		}
		else UARigFunctions::NotificationBox(FString("Open Level Sequence Editor to work with!"));
	}
	else UARigFunctions::NotificationBox(FString("Select source and target actor!"));
}

void UAnimTools::MirrorCurve()
{
	ULevelSequence * Sequencer = Cast <ULevelSequence>(UARigFunctions::GetAssetWithOpenedEditor(ULevelSequence::StaticClass()));
	if (Sequencer->IsValidLowLevel())
	{
		FSelectionIterator Sel = GEditor->GetSelectedActorIterator();
		for (Sel; Sel; ++Sel)
		{
			AActor * SelActor = Cast<AActor >(*Sel);
			if (SelActor->IsValidLowLevel())
			{
				UMovieScene3DTransformSection * TransformSection = UARigFunctions::GetTransformSectionFromActor(Sequencer, SelActor);
				if (TransformSection->IsValidLowLevel())
				{
					GEditor->BeginTransaction(FText::FromString("Mirror Animation"));
					UARigFunctions::MirrorTransformSectionCurve(TransformSection, CurveType, Axis);
					GEditor->EndTransaction();
				}
			}
		}
	}
	else UARigFunctions::NotificationBox(FString("Open Level Sequence Editor to work with!"));
}

void UAnimTools::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty *PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged != nullptr)
	{
		if (PropertyThatChanged->GetFName() == FName("bHide"))
		{
			for (TActorIterator<AActor> It(GEditor->GetEditorWorldContext().World(), ActorClass); It; ++It)
			{
				AActor* Actor = *It;
				Actor->SetIsTemporarilyHiddenInEditor(bHide);
			}
		}
		if (PropertyThatChanged->GetFName() == FName("bSelect"))
		{
			for (TActorIterator<AActor> It(GEditor->GetEditorWorldContext().World(), ActorClass); It; ++It)
			{
				GEditor->SelectActor(*It, bSelect, true, true, true);
			}
		}
		if (PropertyThatChanged->GetFName() == FName("Roll") ||
			PropertyThatChanged->GetFName() == FName("Pitch") ||
			PropertyThatChanged->GetFName() == FName("Yaw") || 
			PropertyThatChanged->GetFName() == FName("RotateRelative"))
		{
			for (int32 i = 0; i < SelectedActors.Num(); i ++)
			{
				SelectedActors[i]->Modify();
				SelectedActors[i]->SetActorRelativeRotation(RotateRelative.Quaternion() * rOffsets[i].Quaternion());
			}
			if (RigActor != nullptr) RigActor->EditorTick();
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UAnimTools::OnSelectionChanged(UObject* Obj)
{
	rOffsets.Reset();
	SelectedActors.Reset();
	RotateRelative = FRotator::ZeroRotator;
	USelection* Selection = Cast<USelection>(Obj);
	if (Selection)
	{
		int32 NumSelected = Selection->Num();
		for (int32 SelectionIndex = 0; SelectionIndex < NumSelected; ++SelectionIndex)
		{
			AActor* Actor = Cast<AActor>(Selection->GetSelectedObject(SelectionIndex));
			if (Actor)
			{
				rOffsets.Add(Actor->GetRootComponent()->RelativeRotation);
				SelectedActors.Add(Actor);
			}
		}
		RigActor = UAllrightRig::GetRigFromSelectedController();
	}
};

void UAnimTools::OnToolTriggered()
{
	USelection::SelectionChangedEvent.AddUObject(this, &UAnimTools::OnSelectionChanged);
}

void UAnimTools::OnToolClosed()
{
	USelection::SelectionChangedEvent.RemoveAll(this);
}

#undef LOCTEXT_NAMESPACE