
#include "ARigRuntimePrivatePCH.h"
#include "ARigFunctions.h"

#define LOCTEXT_NAMESPACE "ARigFunctions"

UARigFunctions::UARigFunctions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

/////////////////////////////////////////////////////// Actor ///////////////////////////////////////////////////////////////////////////////

void UARigFunctions::Parent(AActor *Child, AActor *Parent)
{
	if (Child != nullptr && Parent != nullptr)
	{
		USceneComponent *ChildRootComponent = Child->GetRootComponent();
		USceneComponent *ParentRootComponent = Parent->GetRootComponent();
		FName SocketName;
		if (ChildRootComponent && ParentRootComponent)
		{
			ChildRootComponent->AttachToComponent(ParentRootComponent, FAttachmentTransformRules::KeepWorldTransform, SocketName);
		}
	}
}

void UARigFunctions::RenameActor(AActor *Actor, FName NewName)
{
#if WITH_EDITOR
	if (Actor != nullptr) Actor->SetActorLabel(NewName.ToString(), true);
#endif
}

void UARigFunctions::SelectActorInEditor(AActor *Actor, bool IsSelected)
{
#if WITH_EDITOR
	if (Actor != nullptr) GEditor->SelectActor(Actor, IsSelected, true, true, true);
#endif
}

void UARigFunctions::PostEditChange(AActor *Actor)
{
#if WITH_EDITOR
	if (Actor != nullptr) Actor->PostEditChange();
#endif
}

void UARigFunctions::SetSelectable(UPrimitiveComponent *Component, bool bSelectable)
{
#if WITH_EDITOR
	if (Component != nullptr) Component->bSelectable = bSelectable;
#endif
}

/////////////////////////////////////////////////////// Name ///////////////////////////////////////////////////////////////////////////////
FName UARigFunctions::AppendName(FName A, FName B)
{
	FString StrRes;
	FString StrA = A.ToString();
	FString StrB = B.ToString();
	StrRes.Empty(StrA.Len() + StrB.Len() + 1);
	StrRes += StrA;
	StrRes += StrB;
	return FName(*StrRes);
}

bool UARigFunctions::MatchesWildcard(const FName SourceName, const FName Wildcard, ESearchCase::Type SearchCase)
{
	FString SourceString = SourceName.ToString();
	return SourceString.MatchesWildcard(Wildcard.ToString(), SearchCase);
}

/////////////////////////////////////////////////////// Sequencer ///////////////////////////////////////////////////////////////////////////////
void UARigFunctions::IsSequencerHasBindingForActor(AActor *Actor, ULevelSequence *LevelSequence, FMovieSceneBinding &MovieSceneBinding, bool &Result)
{
#if WITH_EDITOR
	if (Actor != nullptr && LevelSequence != nullptr)
	{
		for (auto b : LevelSequence->MovieScene->GetBindings())
		{
			FGuid Guid = b.GetObjectGuid();
			if (Guid.IsValid())
			{
				UObject * Object = LevelSequence->FindPossessableObject(Guid, GWorld);
				if (Object != nullptr)
				{
					if (Actor == Object)
					{
						MovieSceneBinding = b;
						Result = true;
						return;
					}
				}
			}
		}
	}
#endif
	Result = false;
}

void UARigFunctions::GetTracksFromBinding(FMovieSceneBinding MovieSceneBinding, TArray<UMovieSceneTrack*> &MovieSceneTracks)
{
#if WITH_EDITOR
	MovieSceneTracks = MovieSceneBinding.GetTracks();
#endif
}

void UARigFunctions::GetSectionsFromTrack(UMovieSceneTrack *MovieSceneTrack, TArray<UMovieScene3DTransformSection*> &TransformSections)
{
#if WITH_EDITOR
	if (MovieSceneTrack != nullptr)
	{
		for (auto s : MovieSceneTrack->GetAllSections())
		{
			TransformSections.Add(Cast<UMovieScene3DTransformSection>(s));
		}
	}
#endif
}

void UARigFunctions::GetFloatSectionsFromBinding(FMovieSceneBinding MovieSceneBinding, TArray<UMovieSceneFloatSection*> &FloatSections, TArray <FName> &Properties)
{
#if WITH_EDITOR
	for (UMovieSceneTrack * t : MovieSceneBinding.GetTracks())
	{
		for (UMovieSceneSection * s : t->GetAllSections())
		{
			UMovieSceneFloatTrack * FloatTrack = Cast<UMovieSceneFloatTrack>(t);
			if (FloatTrack != nullptr)
			{
				Properties.Add(FloatTrack->GetPropertyName());
			}
			UMovieSceneFloatSection * FloatSection = Cast<UMovieSceneFloatSection>(s);
			if (FloatSection != nullptr)
			{
				FloatSections.Add(FloatSection);
			}
		}
	}
#endif
}

void UARigFunctions::SetTransformSectionKey(AActor *Actor, UMovieScene3DTransformSection* TransformSection, FTransform Transform, float Time, bool Translation, bool Rotation, bool Scale, bool &Result)
{
#if WITH_EDITOR
	if (TransformSection != nullptr && Actor != nullptr)
	{
		if (Translation)
		{
			Actor->SetActorLocation(Transform.GetTranslation());
			FVector P = Actor->GetRootComponent()->RelativeLocation;

			FKeyHandle txKeyHandle = TransformSection->GetTranslationCurve(EAxis::X).UpdateOrAddKey(Time, P.X);
			TransformSection->GetTranslationCurve(EAxis::X).SetKeyInterpMode(txKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetTranslationCurve(EAxis::X).SetKeyTangentMode(txKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetTranslationCurve(EAxis::X).SetKeyTangentWeightMode(txKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle tyKeyHandle = TransformSection->GetTranslationCurve(EAxis::Y).UpdateOrAddKey(Time, P.Y);
			TransformSection->GetTranslationCurve(EAxis::Y).SetKeyInterpMode(tyKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetTranslationCurve(EAxis::Y).SetKeyTangentMode(tyKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetTranslationCurve(EAxis::Y).SetKeyTangentWeightMode(tyKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle tzKeyHandle = TransformSection->GetTranslationCurve(EAxis::Z).UpdateOrAddKey(Time, P.Z);
			TransformSection->GetTranslationCurve(EAxis::Z).SetKeyInterpMode(tzKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetTranslationCurve(EAxis::Z).SetKeyTangentMode(tzKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetTranslationCurve(EAxis::Z).SetKeyTangentWeightMode(tzKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);
		}
		if (Rotation)
		{
			Actor->SetActorRotation(Transform.GetRotation());
			FRotator R = Actor->GetRootComponent()->RelativeRotation;

			FKeyHandle rxKeyHandle = TransformSection->GetRotationCurve(EAxis::X).UpdateOrAddKey(Time, R.Roll);
			TransformSection->GetRotationCurve(EAxis::X).SetKeyInterpMode(rxKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetRotationCurve(EAxis::X).SetKeyTangentMode(rxKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetRotationCurve(EAxis::X).SetKeyTangentWeightMode(rxKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle ryKeyHandle = TransformSection->GetRotationCurve(EAxis::Y).UpdateOrAddKey(Time, R.Pitch);
			TransformSection->GetRotationCurve(EAxis::Y).SetKeyInterpMode(ryKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetRotationCurve(EAxis::Y).SetKeyTangentMode(ryKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetRotationCurve(EAxis::Y).SetKeyTangentWeightMode(ryKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle rzKeyHandle = TransformSection->GetRotationCurve(EAxis::Z).UpdateOrAddKey(Time, R.Yaw);
			TransformSection->GetRotationCurve(EAxis::Z).SetKeyInterpMode(rzKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetRotationCurve(EAxis::Z).SetKeyTangentMode(rzKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetRotationCurve(EAxis::Z).SetKeyTangentWeightMode(rzKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);
		}
		if (Scale)
		{
			Actor->SetActorScale3D(Transform.GetScale3D());
			FVector S = Actor->GetRootComponent()->RelativeScale3D;

			FKeyHandle sxKeyHandle = TransformSection->GetScaleCurve(EAxis::X).UpdateOrAddKey(Time, S.X);
			TransformSection->GetScaleCurve(EAxis::X).SetKeyInterpMode(sxKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetScaleCurve(EAxis::X).SetKeyTangentMode(sxKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetScaleCurve(EAxis::X).SetKeyTangentWeightMode(sxKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle syKeyHandle = TransformSection->GetScaleCurve(EAxis::Y).UpdateOrAddKey(Time, S.Y);
			TransformSection->GetScaleCurve(EAxis::Y).SetKeyInterpMode(syKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetScaleCurve(EAxis::Y).SetKeyTangentMode(syKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetScaleCurve(EAxis::Y).SetKeyTangentWeightMode(syKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);

			FKeyHandle szKeyHandle = TransformSection->GetScaleCurve(EAxis::Z).UpdateOrAddKey(Time, S.Z);
			TransformSection->GetScaleCurve(EAxis::Z).SetKeyInterpMode(szKeyHandle, ERichCurveInterpMode::RCIM_Cubic);
			TransformSection->GetScaleCurve(EAxis::Z).SetKeyTangentMode(szKeyHandle, ERichCurveTangentMode::RCTM_Auto);
			TransformSection->GetScaleCurve(EAxis::Z).SetKeyTangentWeightMode(szKeyHandle, ERichCurveTangentWeightMode::RCTWM_WeightedBoth);
		}
		Result = true;
		return;
	}
#endif
	Result = false;
}

void UARigFunctions::SetSectionStartTime(UMovieScene3DTransformSection *TransformSection, float Time)
{
#if WITH_EDITOR
	if (TransformSection != nullptr) TransformSection->SetStartTime(Time);
#endif
}

void UARigFunctions::SetSectionEndTime(UMovieScene3DTransformSection *TransformSection, float Time)
{
#if WITH_EDITOR
	if (TransformSection != nullptr) TransformSection->SetEndTime(Time);
#endif
}

void UARigFunctions::GetSequencerComponents(ULevelSequence *LevelSequence, TArray<FMovieSceneBinding> &MovieSceneBindings, TArray<USceneComponent*> &SceneComponents)
{
#if WITH_EDITOR
	if (LevelSequence != nullptr)
	{
		for (auto b : LevelSequence->MovieScene->GetBindings())
		{
			FGuid Guid = b.GetObjectGuid();
			if (Guid.IsValid())
			{
				UObject *Object = LevelSequence->FindPossessableObject(Guid, GWorld);
				if (Object != nullptr)
				{
					USceneComponent* SceneComponent = MovieSceneHelpers::SceneComponentFromRuntimeObject(Object);
					if (SceneComponent != nullptr)
					{
						MovieSceneBindings.Add(b);
						SceneComponents.Add(SceneComponent);
					}
				}
			}
		}
	}
#endif
}

void UARigFunctions::UpdateSequncerComponent(UMovieScene3DTransformSection *TransformSection, USceneComponent *SceneComponent, float Time)
{
#if WITH_EDITOR
	if (SceneComponent != nullptr && TransformSection != nullptr)
	{
		FVector Translation;
		FRotator Rotation;
		FVector Scale;
		Translation.X = TransformSection->GetTranslationCurve(EAxis::X).Eval(Time);
		Translation.Y = TransformSection->GetTranslationCurve(EAxis::Y).Eval(Time);
		Translation.Z = TransformSection->GetTranslationCurve(EAxis::Z).Eval(Time);
		Rotation.Roll = TransformSection->GetRotationCurve(EAxis::X).Eval(Time);
		Rotation.Pitch = TransformSection->GetRotationCurve(EAxis::Y).Eval(Time);
		Rotation.Yaw = TransformSection->GetRotationCurve(EAxis::Z).Eval(Time);
		Scale.X = TransformSection->GetScaleCurve(EAxis::X).Eval(Time);
		Scale.Y = TransformSection->GetScaleCurve(EAxis::Y).Eval(Time);
		Scale.Z = TransformSection->GetScaleCurve(EAxis::Z).Eval(Time);
		SceneComponent->SetRelativeLocationAndRotation(Translation, Rotation);
		SceneComponent->SetRelativeScale3D(Scale);
	}
#endif
}

void UARigFunctions::AddBindingToSequencer(ULevelSequence *LevelSequence, AActor *Actor, FGuid &Guid)
{
#if WITH_EDITOR
	if (LevelSequence != nullptr && Actor != nullptr)
	{
		Guid = LevelSequence->MovieScene->AddPossessable(Actor->GetActorLabel(), Actor->GetClass());
		LevelSequence->BindPossessableObject(Guid, *Actor, Actor->GetWorld());
	}
#endif
}

void UARigFunctions::AddTransformTrack(ULevelSequence *LevelSequence, FGuid Guid, float StartTime, float EndTime, UMovieScene3DTransformSection* &TransformSection)
{
#if WITH_EDITOR
	FName Transform("Transform");
	UMovieSceneTrack* Track = LevelSequence->MovieScene->AddTrack(UMovieScene3DTransformTrack::StaticClass(), Guid);
	UMovieScene3DTransformTrack* TransformTrack = CastChecked<UMovieScene3DTransformTrack>(Track);
	TransformTrack->SetPropertyNameAndPath(Transform, Transform.ToString());
	TransformSection = CastChecked<UMovieScene3DTransformSection>(TransformTrack->CreateNewSection());
	TransformTrack->AddSection(*TransformSection);
	TransformSection->SetStartTime(StartTime);
	TransformSection->SetEndTime(EndTime);
	TransformSection->SetIsInfinite(true);
	TransformSection->GetScaleCurve(EAxis::X).SetDefaultValue(1.0f);
	TransformSection->GetScaleCurve(EAxis::Y).SetDefaultValue(1.0f);
	TransformSection->GetScaleCurve(EAxis::Z).SetDefaultValue(1.0f);
#endif
}

void UARigFunctions::RemoveBindingFromSequencer(ULevelSequence *LevelSequence, AActor *Actor, FMovieSceneBinding MovieSceneBinding)
{
#if WITH_EDITOR
	if (LevelSequence != nullptr && Actor != nullptr)
	{
		FGuid Guid = MovieSceneBinding.GetObjectGuid();
		if (Guid.IsValid())
		{
			LevelSequence->MovieScene->Modify();
			LevelSequence->MovieScene->RemovePossessable(Guid);
		}
	}
#endif
}

UMovieScene3DTransformSection * UARigFunctions::GetTransformSectionFromActor(ULevelSequence *LevelSequence, AActor *Actor)
{
	UMovieScene3DTransformSection *TransformSection = nullptr;
#if WITH_EDITOR
	if (Actor != nullptr && LevelSequence != nullptr)
	{
		for (auto b : LevelSequence->MovieScene->GetBindings())
		{
			FGuid Guid = b.GetObjectGuid();
			if (Guid.IsValid())
			{
				UObject * Object = LevelSequence->FindPossessableObject(Guid, GWorld);
				if (Object != nullptr)
				{
					if (Actor == Object)
					{
						if (b.GetTracks().Num())
						{
							UMovieSceneTrack *MovieSceneTrack = b.GetTracks()[0];
							if (MovieSceneTrack->GetAllSections().Num())
							{
								UMovieSceneSection *SceneSection = MovieSceneTrack->GetAllSections()[0];
								TransformSection = Cast<UMovieScene3DTransformSection>(SceneSection);
							}
						}
						return TransformSection;
					}
				}
			}
		}
	}
#endif
	return TransformSection;
}

TArray <UMovieSceneSkeletalAnimationSection*> UARigFunctions::GetAnimSectionsFromActor(ULevelSequence *LevelSequence, AActor *Actor)
{
	TArray <UMovieSceneSkeletalAnimationSection*> AnimSections;
#if WITH_EDITOR
	if (Actor != nullptr && LevelSequence != nullptr)
	{
		for (auto b : LevelSequence->MovieScene->GetBindings())
		{
			FGuid Guid = b.GetObjectGuid();
			if (Guid.IsValid())
			{
				UObject * Object = LevelSequence->FindPossessableObject(Guid, GWorld);
				if (Object != nullptr)
				{
					if (Actor == Object)
					{
						for (UMovieSceneTrack *MovieSceneTrack : b.GetTracks())
						{
							UMovieSceneSkeletalAnimationTrack * AnimationTrack = Cast<UMovieSceneSkeletalAnimationTrack>(MovieSceneTrack);
							if (AnimationTrack->IsValidLowLevel())
							{
								for (UMovieSceneSection * Section : AnimationTrack->GetAllSections())
								{
									UMovieSceneSkeletalAnimationSection * AnimationSection = Cast<UMovieSceneSkeletalAnimationSection>(Section);
									if (AnimationSection->IsValidLowLevel()) AnimSections.Add(AnimationSection);
								}
							}
						}
					}
				}
			}
		}
	}
#endif
	return AnimSections;
}

void UARigFunctions::CopyActorAnimation(ULevelSequence *LevelSequence, AActor *SourceActor, AActor *TargetActor, float ShiftAnim, bool bMirror, EAxis::Type MirrorAxis)
{
#if WITH_EDITOR
	if (SourceActor->IsValidLowLevel() && TargetActor->IsValidLowLevel() && LevelSequence->IsValidLowLevel())
	{
		UMovieScene3DTransformSection *SourceTransformSection = GetTransformSectionFromActor(LevelSequence, SourceActor);
		UMovieScene3DTransformSection *TargetTransformSection = GetTransformSectionFromActor(LevelSequence, TargetActor);
		if (SourceTransformSection->IsValidLowLevel() && !TargetTransformSection->IsValidLowLevel())
		{
			FGuid Guid;
			AddBindingToSequencer(LevelSequence, TargetActor, Guid);
			AddTransformTrack(LevelSequence, Guid, SourceTransformSection->GetStartTime(), SourceTransformSection->GetEndTime(), TargetTransformSection);
		}
		if (SourceTransformSection->IsValidLowLevel() && TargetTransformSection->IsValidLowLevel())
		{

			TargetTransformSection->SetStartTime(SourceTransformSection->GetStartTime());
			TargetTransformSection->SetEndTime(SourceTransformSection->GetEndTime());

			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Loc, EAxis::X);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Loc, EAxis::Y);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Loc, EAxis::Z);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Rot, EAxis::X);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Rot, EAxis::Y);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Rot, EAxis::Z);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Scale, EAxis::X);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Scale, EAxis::Y);
			CopyTransformSectionCurve(SourceTransformSection, TargetTransformSection, ETransformType::TE_Scale, EAxis::Z);
			
			if (ShiftAnim != 0.0f) ShiftTransformSectionAnim(TargetTransformSection, ShiftAnim);
			
			TargetTransformSection->SetIsInfinite(SourceTransformSection->IsInfinite());
			
			if (bMirror) MirrorTransformSectionAnim(TargetTransformSection, MirrorAxis);
			
			CloseAllEditorsForAsset(LevelSequence);
			OpenEditorForAsset(LevelSequence);
		}
	}
#endif
}

void UARigFunctions::MirrorTransformSectionAnim(UMovieScene3DTransformSection *TransformSection, EAxis::Type MirrorAxis)
{
#if WITH_EDITOR
	if (TransformSection->IsValidLowLevel())
	{
		if (MirrorAxis != EAxis::None)
		{
			for (auto It(TransformSection->GetTranslationCurve(MirrorAxis).GetKeyHandleIterator()); It; ++It)
			{
				FRichCurveKey &CurveKey = TransformSection->GetTranslationCurve(MirrorAxis).GetKey(It.Key());
				CurveKey.Value *= -1.0f;
				CurveKey.ArriveTangent *= -1.0f;
				CurveKey.LeaveTangent *= -1.0f;
			}
			if (MirrorAxis == EAxis::X)
			{
				for (auto It(TransformSection->GetRotationCurve(EAxis::Y).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::Y).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
				for (auto It(TransformSection->GetRotationCurve(EAxis::Z).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::Z).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
			}
			if (MirrorAxis == EAxis::Y)
			{
				for (auto It(TransformSection->GetRotationCurve(EAxis::X).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::X).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
				for (auto It(TransformSection->GetRotationCurve(EAxis::Z).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::Z).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
			}
			if (MirrorAxis == EAxis::Z)
			{
				for (auto It(TransformSection->GetRotationCurve(EAxis::X).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::X).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
				for (auto It(TransformSection->GetRotationCurve(EAxis::Y).GetKeyHandleIterator()); It; ++It)
				{
					FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(EAxis::Y).GetKey(It.Key());
					CurveKey.Value *= -1.0f;
					CurveKey.ArriveTangent *= -1.0f;
					CurveKey.LeaveTangent *= -1.0f;
				}
			}
		}
	}
#endif
}

void UARigFunctions::MirrorTransformSectionCurve(UMovieScene3DTransformSection *TransformSection, ETransformType TransformType, EAxis::Type Axis /*= EAxis::X*/)
{
#if WITH_EDITOR
	if (TransformSection->IsValidLowLevel())
	{
		TransformSection->Modify();
		if (TransformType == ETransformType::TE_Loc)
		{
			for (auto It(TransformSection->GetTranslationCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				FRichCurveKey &CurveKey = TransformSection->GetTranslationCurve(Axis).GetKey(It.Key());
				CurveKey.Value *= -1.0f;
				CurveKey.ArriveTangent *= -1.0f;
				CurveKey.LeaveTangent *= -1.0f;
			}
		}
		if (TransformType == ETransformType::TE_Rot)
		{
			for (auto It(TransformSection->GetRotationCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				FRichCurveKey &CurveKey = TransformSection->GetRotationCurve(Axis).GetKey(It.Key());
				CurveKey.Value *= -1.0f;
				CurveKey.ArriveTangent *= -1.0f;
				CurveKey.LeaveTangent *= -1.0f;
			}
		}
		if (TransformType == ETransformType::TE_Scale)
		{
			for (auto It(TransformSection->GetScaleCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				FRichCurveKey &CurveKey = TransformSection->GetScaleCurve(Axis).GetKey(It.Key());
				CurveKey.Value *= -1.0f;
				CurveKey.ArriveTangent *= -1.0f;
				CurveKey.LeaveTangent *= -1.0f;
			}
		}
	}
#endif
}

void UARigFunctions::ShiftTransformSectionAnim(UMovieScene3DTransformSection *TransformSection, float ShiftAnim)
{
#if WITH_EDITOR
	if (TransformSection->IsValidLowLevel())
	{
		TransformSection->SetStartTime(TransformSection->GetStartTime() + ShiftAnim);
		TransformSection->SetEndTime(TransformSection->GetEndTime() + ShiftAnim);

		TransformSection->GetTranslationCurve(EAxis::X).ShiftCurve(ShiftAnim);
		TransformSection->GetTranslationCurve(EAxis::Y).ShiftCurve(ShiftAnim);
		TransformSection->GetTranslationCurve(EAxis::Z).ShiftCurve(ShiftAnim);
		TransformSection->GetRotationCurve(EAxis::X).ShiftCurve(ShiftAnim);
		TransformSection->GetRotationCurve(EAxis::Y).ShiftCurve(ShiftAnim);
		TransformSection->GetRotationCurve(EAxis::Z).ShiftCurve(ShiftAnim);
		TransformSection->GetScaleCurve(EAxis::X).ShiftCurve(ShiftAnim);
		TransformSection->GetScaleCurve(EAxis::Y).ShiftCurve(ShiftAnim);
		TransformSection->GetScaleCurve(EAxis::Z).ShiftCurve(ShiftAnim);
	}
#endif
}

void UARigFunctions::CopyTransformSectionCurve(UMovieScene3DTransformSection *SourceTransformSection, UMovieScene3DTransformSection *TargetTransformSection, ETransformType TransformType, EAxis::Type Axis /*= EAxis::X*/)
{
#if WITH_EDITOR
	if (SourceTransformSection->IsValidLowLevel() && TargetTransformSection->IsValidLowLevel())
	{
		if (TransformType == ETransformType::TE_Loc)
		{
			TargetTransformSection->GetTranslationCurve(Axis).Keys = SourceTransformSection->GetTranslationCurve(Axis).Keys;
			TargetTransformSection->GetTranslationCurve(Axis).PreInfinityExtrap = SourceTransformSection->GetTranslationCurve(Axis).PreInfinityExtrap;
			TargetTransformSection->GetTranslationCurve(Axis).PostInfinityExtrap = SourceTransformSection->GetTranslationCurve(Axis).PostInfinityExtrap;
			for (auto It(TargetTransformSection->GetTranslationCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				TargetTransformSection->GetTranslationCurve(Axis).GetKey(It.Key());
			}
		}
		if (TransformType == ETransformType::TE_Rot)
		{
			TargetTransformSection->GetRotationCurve(Axis).Keys = SourceTransformSection->GetRotationCurve(Axis).Keys;
			TargetTransformSection->GetRotationCurve(Axis).PreInfinityExtrap = SourceTransformSection->GetRotationCurve(Axis).PreInfinityExtrap;
			TargetTransformSection->GetRotationCurve(Axis).PostInfinityExtrap = SourceTransformSection->GetRotationCurve(Axis).PostInfinityExtrap;
			for (auto It(TargetTransformSection->GetRotationCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				TargetTransformSection->GetRotationCurve(Axis).GetKey(It.Key());
			}
		}
		if (TransformType == ETransformType::TE_Scale)
		{
			TargetTransformSection->GetScaleCurve(Axis).Keys = SourceTransformSection->GetScaleCurve(Axis).Keys;
			TargetTransformSection->GetScaleCurve(Axis).PreInfinityExtrap = SourceTransformSection->GetScaleCurve(Axis).PreInfinityExtrap;
			TargetTransformSection->GetScaleCurve(Axis).PostInfinityExtrap = SourceTransformSection->GetScaleCurve(Axis).PostInfinityExtrap;
			for (auto It(TargetTransformSection->GetScaleCurve(Axis).GetKeyHandleIterator()); It; ++It)
			{
				TargetTransformSection->GetScaleCurve(Axis).GetKey(It.Key());
			}
		}
	}
#endif
}

int32 UARigFunctions::GetSequencerFrameRate(ULevelSequence *LevelSequence)
{
	int32 Fps = 0;
#if WITH_EDITOR
	if (LevelSequence->IsValidLowLevel()) Fps = FMath::RoundToInt(1 / LevelSequence->MovieScene->GetFixedFrameInterval());
#endif
	return Fps;
}

void UARigFunctions::UpdateFloatSection(UMovieSceneFloatSection * FloatSection, AActor *Actor, FName PropertyName, float Time)
{
#if WITH_EDITOR
	if (FloatSection != nullptr && Actor != nullptr)
	{
		float Value = FloatSection->GetFloatCurve().Eval(Time);

		UFloatProperty *FloatProp = FindField<UFloatProperty>(Actor->GetClass(), PropertyName);
		if (FloatProp != nullptr)
		{
			FloatProp->SetPropertyValue_InContainer(Actor, Value);
			Actor->PostEditChange();
		}
	}
#endif
}

void UARigFunctions::UpdateSkeletalMeshSection(ULevelSequence *LevelSequence, ASkeletalMeshActor *SkelMeshActor, float Time)
{
#if WITH_EDITOR
	if (LevelSequence != nullptr && SkelMeshActor != nullptr)
	{

		USkeletalMeshComponent * SkelMeshComp = SkelMeshActor->GetSkeletalMeshComponent();
		SkelMeshComp->InitializeAnimScriptInstance();
		TArray <UMovieSceneSkeletalAnimationSection*> SkelAnimSections = GetAnimSectionsFromActor(LevelSequence, SkelMeshActor);
		for (int32 i = 0; i < SkelAnimSections.Num(); i ++)
		{
			FMovieSceneSkeletalAnimationParams Params = SkelAnimSections[i]->Params;
			PreviewSetAnimPosition(SkelMeshComp, i, Params.Animation, (Time - SkelAnimSections[i]->GetStartTime()), Params.Weight.Eval(Time));
		}
	}
#endif
}

void UARigFunctions::PreviewSetAnimPosition(USkeletalMeshComponent* SkelMeshComp, int32 SequenceIndex, UAnimSequenceBase* InAnimSequence, float InPosition, float Weight)
{
#if WITH_EDITOR
	if (SkelMeshComp != nullptr && InAnimSequence != nullptr)
	{
		UAnimSequencerInstance* SequencerInst = Cast<UAnimSequencerInstance>(SkelMeshComp->GetAnimInstance());
		if (SequencerInst)
		{
			SequencerInst->UpdateAnimTrack(InAnimSequence, SequenceIndex, InPosition, Weight, false);
			SkelMeshComp->TickAnimation(0.03f, false);
			SkelMeshComp->RefreshBoneTransforms();
			SkelMeshComp->RefreshSlaveComponents();
			SkelMeshComp->UpdateComponentToWorld();
			SkelMeshComp->FinalizeBoneTransform();
			SkelMeshComp->MarkRenderTransformDirty();
			SkelMeshComp->MarkRenderDynamicDataDirty();
		}
	}
#endif
}

void UARigFunctions::GetSequencerPlaybackRange(ULevelSequence *LevelSequence, float &StartFrame, float &EndFrame)
{
	if (LevelSequence != nullptr)
	{
		StartFrame = LevelSequence->MovieScene->GetPlaybackRange().GetLowerBoundValue();
		EndFrame = LevelSequence->MovieScene->GetPlaybackRange().GetUpperBoundValue();
	}
}

/////////////////////////////////////////////////////// Recorder ///////////////////////////////////////////////////////////////////////////////
void UARigFunctions::AddRawKeyFromPose(USkeletalMeshComponent *SkeletalMeshComponent, UAnimSequence *AnimSequence, int32 FrameRate)
{
#if WITH_EDITOR
	if (SkeletalMeshComponent != nullptr && AnimSequence != nullptr)
	{
		TArray<FTransform> Transforms = SkeletalMeshComponent->GetComponentSpaceTransforms();
		USkeleton* AnimSkeleton = AnimSequence->GetSkeleton();
		for (int32 i = 0; i < AnimSequence->GetRawAnimationData().Num(); ++i)
		{
			int32 BoneTreeIndex = AnimSequence->GetSkeletonIndexFromRawDataTrackIndex(i);
			if (BoneTreeIndex != INDEX_NONE)
			{
				int32 BoneIndex = AnimSkeleton->GetMeshBoneIndexFromSkeletonBoneIndex(SkeletalMeshComponent->SkeletalMesh, BoneTreeIndex);
				int32 ParentIndex = SkeletalMeshComponent->SkeletalMesh->RefSkeleton.GetParentIndex(BoneIndex);
				FTransform LocalTransform = Transforms[BoneIndex];
				if (ParentIndex != INDEX_NONE)
				{
					LocalTransform.SetToRelativeTransform(Transforms[ParentIndex]);
				}
				FRawAnimSequenceTrack& RawTrack = AnimSequence->GetRawAnimationTrack(i);
				RawTrack.PosKeys.Add(LocalTransform.GetTranslation());
				RawTrack.RotKeys.Add(LocalTransform.GetRotation());
				RawTrack.ScaleKeys.Add(LocalTransform.GetScale3D());
			}
		}
		int32 LastFrame = AnimSequence->GetRawAnimationTrack(0).PosKeys.Num();
		AnimSequence->NumFrames = LastFrame;
		AnimSequence->SequenceLength = float(LastFrame - 1) / float(FrameRate);
	}
#endif
}

void UARigFunctions::CreateAnimSequence(USkeletalMeshComponent* SkeletalMeshComponent, UAnimSequence* &AnimSequence)
{
#if WITH_EDITOR
	if (SkeletalMeshComponent != nullptr)
	{
		USkeleton *Skeleton = SkeletalMeshComponent->SkeletalMesh->Skeleton;
		FString AssetName;
		FString AssetPath = Skeleton->GetOutermost()->GetName();
		TSharedRef<SCreateAnimAssetWindow> NewAnimDlg =
			SNew(SCreateAnimAssetWindow)
			.DefaultAssetPath(FText::FromString(AssetPath));
		if (NewAnimDlg->ShowModal() == EAppReturnType::Cancel) return;
		AssetPath = NewAnimDlg->GetFullAssetPath();
		AssetName = NewAnimDlg->GetAssetName();
		UObject *Parent = CreatePackage(nullptr, *AssetPath);
		UObject* const Object = LoadObject<UObject>(Parent, *AssetName, nullptr, LOAD_None, nullptr);
		if (Object)
		{
			FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("UnrealEd", "Error_AssetExist", "Asset with same name exists. Can't overwrite another asset"));
			return;
		}
		UAnimSequence* NewSeq = NewObject<UAnimSequence>(Parent, *AssetName, RF_Public | RF_Standalone);
		if (NewSeq)
		{
			NewSeq->SetSkeleton(Skeleton);
			FAssetRegistryModule::AssetCreated(NewSeq);

			AnimSequence = NewSeq;
			AnimSequence->SequenceLength = 0.f;
			AnimSequence->NumFrames = 0;
			for (int32 b = 0; b < SkeletalMeshComponent->GetNumBones(); ++b)
			{
				int32 BoneTreeIndex = Skeleton->GetSkeletonBoneIndexFromMeshBoneIndex(SkeletalMeshComponent->MasterPoseComponent != nullptr ? SkeletalMeshComponent->MasterPoseComponent->SkeletalMesh : SkeletalMeshComponent->SkeletalMesh, b);
				if (BoneTreeIndex != INDEX_NONE)
				{
					FName BoneTreeName = Skeleton->GetReferenceSkeleton().GetBoneName(BoneTreeIndex);
					AnimSequence->AddNewRawTrack(BoneTreeName);
				}
			}
			AnimSequence->InitializeNotifyTrack();
		}
	}
#endif
}

void UARigFunctions::PostProcessAnimSequence(UAnimSequence* AnimSequence, USkeletalMeshComponent* SkeletalMeshComponent)
{
#if WITH_EDITOR
	if (AnimSequence != nullptr && SkeletalMeshComponent != nullptr)
	{
		if (AnimSequence->NumFrames == 0)
		{
			AnimSequence->CreateAnimation(SkeletalMeshComponent->SkeletalMesh);
		}
		AnimSequence->PostProcessSequence();
		AnimSequence->MarkPackageDirty();
	}
#endif
}

void UARigFunctions::GetSpaceBases(USkinnedMeshComponent *SkinnedMeshComponent, TArray<FTransform> &SpaceBases)
{
#if WITH_EDITOR
	if (SkinnedMeshComponent != nullptr) SpaceBases = SkinnedMeshComponent->GetComponentSpaceTransforms();
#endif
}

void UARigFunctions::AddRawMorphTargetKey(UAnimSequence *AnimSequence, FName TargetName, float Time, float Weight)
{
#if WITH_EDITOR
	if (AnimSequence != nullptr)
	{
		USkeleton* Skeleton = AnimSequence->GetSkeleton();
		FSmartName NewName;
		Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, TargetName, NewName);
		FFloatCurve *Curve = static_cast<FFloatCurve *>(AnimSequence->RawCurveData.GetCurveData(NewName.UID, FRawCurveTracks::FloatType));
		if (Curve == NULL)
		{
			if (AnimSequence->RawCurveData.AddCurveData(NewName, 0x00000001))
			{
				Curve = static_cast<FFloatCurve *>(AnimSequence->RawCurveData.GetCurveData(NewName.UID, FRawCurveTracks::FloatType));
				FFloatCurve &AnimCurve = *Curve;
				AnimCurve.FloatCurve.UpdateOrAddKey(Time, Weight);
				AnimSequence->MarkRawDataAsModified();
				AnimSequence->MarkPackageDirty();
			}
		}
		else
		{
			FFloatCurve &AnimCurve = *Curve;
			AnimCurve.FloatCurve.UpdateOrAddKey(Time, Weight);
			AnimSequence->MarkRawDataAsModified();
			AnimSequence->MarkPackageDirty();
		}
	}
#endif
}

/////////////////////////////////////////////////////// Editor ///////////////////////////////////////////////////////////////////////////////
void UARigFunctions::NotificationBox(FString String, float FadeIn, float Expire, float FadeOut)
{
#if WITH_EDITOR
	const FText Message = FText::Format(LOCTEXT("NotificationInfo", "{0}"), FText::FromString(String));
	FNotificationInfo Info(Message);
	Info.FadeInDuration = FadeIn;
	Info.ExpireDuration = Expire;
	Info.FadeOutDuration = FadeOut;
	Info.bUseThrobber = false;
	Info.bUseSuccessFailIcons = true;
	Info.bUseLargeFont = true;
	Info.bFireAndForget = false;
	Info.bAllowThrottleWhenFrameRateIsLow = false;
	auto NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
	NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
	NotificationItem->ExpireAndFadeout();
#endif
}

void UARigFunctions::OpenEditorForAsset(UObject *Asset)
{
#if WITH_EDITOR
	if (Asset != nullptr) FAssetEditorManager::Get().OpenEditorForAsset(Asset);
#endif
}

void UARigFunctions::CloseAllEditorsForAsset(UObject *Asset)
{
#if WITH_EDITOR
	if (Asset != nullptr) FAssetEditorManager::Get().CloseAllEditorsForAsset(Asset);
#endif
}

bool UARigFunctions::PromtDialogue(FString Title, FString Message, FString Ok, FString Cancel, TArray<UObject*> Objects)
{
	bool Result = false;
#if WITH_EDITOR
	const FText TextTitle = FText::Format(NSLOCTEXT("PackagesDialogModule", "PackagesDialogTitle", "{0}"), FText::FromString(Title));
	const FText TextMessage = FText::Format(NSLOCTEXT("PackagesDialogModule", "PackagesDialogMessage", "{0}"), FText::FromString(Message));
	const FText TextOk = FText::Format(NSLOCTEXT("PackagesDialogModule", "OkButton", "{0}"), FText::FromString(Ok));
	const FText TextCancel = FText::Format(NSLOCTEXT("PackagesDialogModule", "CancelButton", "{0}"), FText::FromString(Cancel));
	FPackagesDialogModule& PackagesDialogModule = FModuleManager::LoadModuleChecked<FPackagesDialogModule>(TEXT("PackagesDialog"));
	PackagesDialogModule.CreatePackagesDialog(TextTitle, TextMessage, true);
	PackagesDialogModule.AddButton(DRT_Save, TextOk, NSLOCTEXT("PackagesDialogModule", "OkButtonTip", "True"));
	PackagesDialogModule.AddButton(DRT_Cancel, TextCancel, NSLOCTEXT("PackagesDialogModule", "CancelButtonTip", "False"));
	for (int32 i = 0; i < Objects.Num(); i++)
	{
		UPackage *Package = Objects[i]->GetOutermost();
		PackagesDialogModule.AddPackageItem(Package, Package->GetName(), ECheckBoxState::Checked);
	}
	const EDialogReturnType UserResponse = PackagesDialogModule.ShowPackagesDialog();
	if (UserResponse == DRT_Save) Result = true;
#endif
	return Result;
}

void UARigFunctions::GetAssetsByClass(UClass *Class, TArray<UObject*> &Objects)
{
#if WITH_EDITOR
	if (Class != nullptr)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		FString ClassName = Class->GetName();
		AssetRegistryModule.Get().GetAssetsByClass(FName(*ClassName), AssetData);
		for (int32 i = 0; i < AssetData.Num(); i++) Objects.Add(AssetData[i].GetAsset());
	}
#endif
}

UObject * UARigFunctions::GetAssetWithOpenedEditor(UClass *Class)
{
	UObject * ReturnAsset = nullptr;
#if WITH_EDITOR
	if (Class->IsValidLowLevel())
	{
		TArray <UObject*> Assets;
		GetAssetsByClass(Class, Assets);
		TArray <IAssetEditorInstance*> AssetEditorInstances;
		for (UObject *Asset : Assets)
		{
			IAssetEditorInstance *AssetEditorInstance = FAssetEditorManager::Get().FindEditorForAsset(Asset, true);
			if (AssetEditorInstance != nullptr) ReturnAsset = Asset;
		}
	}
#endif
	return ReturnAsset;
}

FTransform UARigFunctions::ConvertTransformToRelativeReverse(FTransform WorldTransform, FTransform LocalTransform)
{
	return LocalTransform.GetRelativeTransformReverse(WorldTransform);
}

void UARigFunctions::ModifyObject(UObject *Object)
{
#if WITH_EDITOR
	if (Object != nullptr) Object->Modify();
#endif
}

/////////////////////////////////////////////////////// Animation ///////////////////////////////////////////////////////////////////////////////
void UARigFunctions::TwoBoneIk(
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
	FTransform &LowBone)
{
	const FVector UpBoneScaleInit = InUpBone.GetScale3D();
	if (bAllowStretching)
	{
		InUpBone.SetScale3D(RemapVectorFromAim(StretchScale, FVector(1.f, 1.f, 1.f), UpAimSettings));
	}
	const FVector Effector = EffectorPos.GetTranslation();
	const FVector RootPos = InUpBone.GetTranslation();
	const FTransform InMidBone = MidBoneLocal * InUpBone;
	const FTransform InLowBone = LowBoneLocal * InMidBone;

	const FVector InitialJointPos = InMidBone.GetTranslation();
	const FVector InitialEndPos = InLowBone.GetTranslation();
	FVector DesiredPos = Effector;
	FVector DesiredDelta = DesiredPos - RootPos;
	float DesiredLength = DesiredDelta.Size();
	FVector	DesiredDir;
	if (DesiredLength < (float)KINDA_SMALL_NUMBER)
	{
		DesiredLength = (float)KINDA_SMALL_NUMBER;
		DesiredDir = FVector(1, 0, 0);
	}
	else DesiredDir = DesiredDelta / DesiredLength;
	FVector	JointTargetPos = PoleVector;
	FVector JointTargetDelta = JointTargetPos - RootPos;
	float JointTargetLength = JointTargetDelta.Size();
	FVector JointPlaneNormal, JointBendDir;
	if (JointTargetLength < (float)KINDA_SMALL_NUMBER)
	{
		JointBendDir = FVector(0, 1, 0);
		JointPlaneNormal = FVector(0, 0, 1);
	}
	else
	{
		JointPlaneNormal = DesiredDir ^ JointTargetDelta;
		if (JointPlaneNormal.Size() < (float)KINDA_SMALL_NUMBER)
		{
			DesiredDir.FindBestAxisVectors(JointPlaneNormal, JointBendDir);
		}
		else
		{
			JointPlaneNormal.Normalize();
			JointBendDir = JointTargetDelta - ((JointTargetDelta | DesiredDir) * DesiredDir);
			JointBendDir.Normalize();
		}
	}
	float LowerLimbLength = (InitialEndPos - InitialJointPos).Size();
	float UpperLimbLength = (InitialJointPos - RootPos).Size();
	float MaxLimbLength = LowerLimbLength + UpperLimbLength;
	float ScalingFactor = 1.f;
	if (bAllowStretching)
	{
		if (DesiredLength > MaxLimbLength)
		{
			ScalingFactor = DesiredLength / MaxLimbLength;
			LowerLimbLength *= ScalingFactor;
			UpperLimbLength *= ScalingFactor;
		}
	}
	FVector OutEndPos = DesiredPos;
	FVector OutJointPos = InitialJointPos;
	if (DesiredLength > MaxLimbLength)
	{
		OutJointPos = RootPos + (UpperLimbLength * DesiredDir);
		OutEndPos = RootPos + ((UpperLimbLength + LowerLimbLength) * DesiredDir);
	}
	else
	{
		const float TwoAB = 2.f * UpperLimbLength * DesiredLength;
		const float CosAngle = (TwoAB != 0.f) ? ((UpperLimbLength*UpperLimbLength) + (DesiredLength*DesiredLength) - (LowerLimbLength*LowerLimbLength)) / TwoAB : 0.f;
		const bool bReverseUpperBone = (CosAngle < 0.f);
		if ((CosAngle > 1.f) || (CosAngle < -1.f))
		{
			if (UpperLimbLength > LowerLimbLength)
			{
				OutJointPos = RootPos + (UpperLimbLength * DesiredDir);
				OutEndPos = OutJointPos - (LowerLimbLength * DesiredDir);
			}
			else
			{
				OutJointPos = RootPos - (UpperLimbLength * DesiredDir);
				OutEndPos = OutJointPos + (LowerLimbLength * DesiredDir);
			}
		}
		else
		{
			const float Angle = FMath::Acos(CosAngle);
			const float JointLineDist = UpperLimbLength * FMath::Sin(Angle);
			const float ProjJointDistSqr = (UpperLimbLength*UpperLimbLength) - (JointLineDist*JointLineDist);
			float ProjJointDist = (ProjJointDistSqr > 0.f) ? FMath::Sqrt(ProjJointDistSqr) : 0.f;
			if (bReverseUpperBone) ProjJointDist *= -1.f;
			OutJointPos = RootPos + (ProjJointDist * DesiredDir) + (JointLineDist * JointBendDir);
		}
	}
	FRotator UpBoneRot;
	FRotator MidBoneRot;
	if (!bUseAimSettings)
	{
		{
			FVector const OldDir = (InitialJointPos - RootPos).GetSafeNormal();
			FVector const NewDir = (OutJointPos - RootPos).GetSafeNormal();
			FQuat const DeltaRotation = FQuat::FindBetween(OldDir, NewDir);
			FQuat const OutRotator = DeltaRotation * InUpBone.GetRotation();
			UpBoneRot = OutRotator.Rotator();
		}
		{
			FVector const OldDir = (InitialEndPos - InitialJointPos).GetSafeNormal();
			FVector const NewDir = (OutEndPos - OutJointPos).GetSafeNormal();
			FQuat const DeltaRotation = FQuat::FindBetween(OldDir, NewDir);
			FQuat const OutRotator = DeltaRotation * InMidBone.GetRotation();
			MidBoneRot = OutRotator.Rotator();
		}
	}
	else
	{
		FVector PvPos;
		GetPoleVectorLocation(RootPos, PoleVector, Effector, 1000.f, PvPos);
		AimConstraint(OutJointPos, PvPos, RootPos, UpAimSettings, UpBoneRot);
		AimConstraint(OutEndPos, PvPos, OutJointPos, MidAimSettings, MidBoneRot);
	}
	if (bAllowStretching)
	{
		const FVector StretchFix = RemapVectorFromAim(StretchScale, UpBoneScaleInit, UpAimSettings) - FVector(1.f, 1.f, 1.f);
		const FVector UpScale = RemapVectorFromAim(ScalingFactor, FVector(1.f, 1.f, 1.f), UpAimSettings) + StretchFix;
		const FVector MidScale = RemapVectorFromAim(ScalingFactor, FVector(1.f, 1.f, 1.f), MidAimSettings) + StretchFix;
		UpBone = FTransform(UpBoneRot, RootPos, UpScale);
		MidBone = FTransform(MidBoneRot, OutJointPos, MidScale);
	}
	else
	{
		UpBone = FTransform(UpBoneRot, RootPos, InUpBone.GetScale3D());
		MidBone = FTransform(MidBoneRot, OutJointPos, InUpBone.GetScale3D());
	}
	LowBone = FTransform(EffectorPos.Rotator(), OutEndPos, EffectorPos.GetScale3D());
}

void UARigFunctions::GetPoleVectorLocation(FVector Up, FVector Mid, FVector End, float Multiply, FVector &PvPos)
{
	const float A = (Up - Mid).Size();
	const float B = (Mid - End).Size();
	const float C = (Up - End).Size();
	const float Alpha = FMath::Abs((B * B - C * C - A * A) / (2 * C * C));
	const FVector LerpVector = FMath::Lerp(Up, End, Alpha);
	PvPos = (Mid - LerpVector).GetSafeNormal();
	PvPos *= Multiply;
	PvPos += LerpVector;
}

void UARigFunctions::AimConstraint(FVector Aim, FVector Up, FVector Target, FAimSettings AimSettings, FRotator &Rotation)
{
	FVector AimVector = Aim - Target;
	FVector UpVector = Up - Target;
	if (AimSettings.AimInvert) AimVector *= -1;
	if (AimSettings.UpInvert) UpVector *= -1;
	if (AimSettings.AxisEnum == EAxisEnum::VE_XY) Rotation = FRotationMatrix::MakeFromXY(AimVector, UpVector).Rotator();
	if (AimSettings.AxisEnum == EAxisEnum::VE_XZ) Rotation = FRotationMatrix::MakeFromXZ(AimVector, UpVector).Rotator();
	if (AimSettings.AxisEnum == EAxisEnum::VE_YX) Rotation = FRotationMatrix::MakeFromYX(AimVector, UpVector).Rotator();
	if (AimSettings.AxisEnum == EAxisEnum::VE_YZ) Rotation = FRotationMatrix::MakeFromYZ(AimVector, UpVector).Rotator();
	if (AimSettings.AxisEnum == EAxisEnum::VE_ZX) Rotation = FRotationMatrix::MakeFromZX(AimVector, UpVector).Rotator();
	if (AimSettings.AxisEnum == EAxisEnum::VE_ZY) Rotation = FRotationMatrix::MakeFromZY(AimVector, UpVector).Rotator();
	if (AimSettings.Offset != FRotator(0.0f, 0.0f, 0.0f))
	{
		FQuat AQuat = FQuat(AimSettings.Offset);
		FQuat BQuat = FQuat(Rotation);
		Rotation = FRotator(BQuat*AQuat);
	}
}

FVector UARigFunctions::RemapVectorFromAim(float RollValue, FVector InVector, FAimSettings AimSettings)
{
	FVector OutVector = InVector;
	if (AimSettings.AxisEnum == EAxisEnum::VE_XY) OutVector = FVector(RollValue, InVector.Y, InVector.Z);
	if (AimSettings.AxisEnum == EAxisEnum::VE_XZ) OutVector = FVector(RollValue, InVector.Y, InVector.Z);
	if (AimSettings.AxisEnum == EAxisEnum::VE_YX) OutVector = FVector(InVector.X, RollValue, InVector.Z);
	if (AimSettings.AxisEnum == EAxisEnum::VE_YZ) OutVector = FVector(InVector.X, RollValue, InVector.Z);
	if (AimSettings.AxisEnum == EAxisEnum::VE_ZX) OutVector = FVector(InVector.X, InVector.Y, RollValue);
	if (AimSettings.AxisEnum == EAxisEnum::VE_ZY) OutVector = FVector(InVector.X, InVector.Y, RollValue);
	return OutVector;
}

void UARigFunctions::GetRigMapping(USkeleton *Skeleton, TArray <FName> BaseNames, TArray <FName> &RigNames)
{
#if WITH_EDITOR
	if (Skeleton != nullptr)
	{
		for (int32 i = 0; i < BaseNames.Num(); i++)
		{
			FName RigName = Skeleton->GetRigBoneMapping(BaseNames[i]);
			RigNames.Add(RigName);
		}
	}
#endif
}

void UARigFunctions::UpdateSkeletalMeshPose(USkeletalMeshComponent *SkeletalMeshComponent)
{
#if WITH_EDITOR
	if (SkeletalMeshComponent != nullptr && !SkeletalMeshComponent->GetWorld()->IsGameWorld())
	{
		SkeletalMeshComponent->TickAnimation(0.f, false);
		SkeletalMeshComponent->RefreshBoneTransforms();
		SkeletalMeshComponent->RefreshSlaveComponents();
		SkeletalMeshComponent->UpdateComponentToWorld();
		SkeletalMeshComponent->FinalizeBoneTransform();
		SkeletalMeshComponent->MarkRenderTransformDirty();
		SkeletalMeshComponent->MarkRenderDynamicDataDirty();
	}
#endif
}

void UARigFunctions::SetMorphTarget(USkeletalMeshComponent *SkeletalMeshComponent, FName TargetName, float Weight)
{
#if WITH_EDITOR
	if (SkeletalMeshComponent != nullptr)
	{
		TMap <FName, float> MorphTargetCurves;
		MorphTargetCurves.Add(TargetName, Weight);
		USkeletalMesh *InSkeletalMesh = SkeletalMeshComponent->SkeletalMesh;
		SkeletalMeshComponent->MorphTargetWeights.SetNumZeroed(InSkeletalMesh->MorphTargets.Num());
		if (FMath::Abs(Weight) > SMALL_NUMBER)
		{
			int32 MorphId = INDEX_NONE;
			UMorphTarget* Target = InSkeletalMesh ? InSkeletalMesh->FindMorphTargetAndIndex(TargetName, MorphId) : nullptr;
			if (Target != nullptr)
			{
				int32 MorphIndex = INDEX_NONE;
				for (int32 i = 0; i < SkeletalMeshComponent->ActiveMorphTargets.Num(); i++)
				{
					if (SkeletalMeshComponent->ActiveMorphTargets[i].MorphTarget == Target)
					{
						MorphIndex = i;
					}
				}
				if (MorphIndex == INDEX_NONE)
				{
					SkeletalMeshComponent->ActiveMorphTargets.Add(FActiveMorphTarget(Target, MorphId));
					SkeletalMeshComponent->MorphTargetWeights[MorphId] = Weight;
				}
				else
				{
					check(MorphId == SkeletalMeshComponent->ActiveMorphTargets[MorphIndex].WeightIndex);
					float& CurrentWeight = SkeletalMeshComponent->MorphTargetWeights[MorphId];
					CurrentWeight = FMath::Max<float>(CurrentWeight, Weight);
				}
			}
		}
	}
#endif
}

FRotator UARigFunctions::GetWeightedAverageRotator(TArray <FRotator> Rotators, TArray <float> Weights)
{
	FQuat OutRot = FQuat::Identity;
	if (Rotators.Num() && Rotators.Num() == Weights.Num())
	{
		float WeightsSum = 0.0f;
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
		for (int i = 0; i < Rotators.Num(); i++)
		{
			FQuat q = Rotators[i].Quaternion();
			x += q.X * Weights[i];
			y += q.Y * Weights[i];
			z += q.Z * Weights[i];
			w += q.W * Weights[i];
			WeightsSum += Weights[i];
		}
		if (WeightsSum > KINDA_SMALL_NUMBER)
		{
			float k = 1.0f / FMath::Sqrt(x * x + y * y + z * z + w * w);
			OutRot = FQuat(x * k, y * k, z * k, w * k);
		}
	}
	return OutRot.Rotator();
}

FVector UARigFunctions::GetWeightedAverageVector(TArray <FVector> Vectors, TArray <float> Weights)
{
	FVector OutVector = FVector::ZeroVector;
	if (Vectors.Num() && Vectors.Num() == Weights.Num())
	{
		FVector vSum = FVector::ZeroVector;
		float WeightsSum = 0.0f;
		for (int i = 0; i < Vectors.Num(); i++)
		{
			vSum += Vectors[i] * Weights[i];
			WeightsSum += Weights[i];
		}
		if (WeightsSum > KINDA_SMALL_NUMBER) OutVector = vSum / WeightsSum;
	}
	return OutVector;
}

FTransform UARigFunctions::GetWeightedAverageTransform(TArray <FTransform> Transforms, TArray <FTransform> Offsets, TArray <float> Weights)
{
	FTransform OutTransform;
	if (Transforms.Num() == Offsets.Num() && Offsets.Num() == Weights.Num())
	{
		float WeightsSum = 0.0f;
		FVector vPosSum = FVector::ZeroVector;
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
		FVector vScaleSum = FVector::ZeroVector;
		for (int i = 0; i < Transforms.Num(); i++)
		{
			FTransform T =  Offsets[i] * Transforms[i];

			vPosSum += T.GetLocation() * Weights[i];

			FQuat q = T.GetRotation();
			x += q.X * Weights[i];
			y += q.Y * Weights[i];
			z += q.Z * Weights[i];
			w += q.W * Weights[i];

			vScaleSum += T.GetScale3D() * Weights[i];

			WeightsSum += Weights[i];
		}
		if (WeightsSum > KINDA_SMALL_NUMBER)
		{
			FVector OutPos = vPosSum / WeightsSum;

			float k = 1.0f / FMath::Sqrt(x * x + y * y + z * z + w * w);
			FQuat OutRot = FQuat(x * k, y * k, z * k, w * k);

			FVector OutScale = vScaleSum / WeightsSum;

			OutTransform = FTransform(OutRot, OutPos, OutScale);
		}
	}
	return OutTransform;
}

void UARigFunctions::GetClosestAxis(FVector TargetVector, FTransform Transform, FString &FirstAxis, FString &SecondAxis)
{
	FRotationMatrix RM(Transform.Rotator());
	TArray <FVector> vRM = {FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector};
	RM.GetScaledAxes(vRM[0], vRM[1], vRM[2]);
	vRM.Add(-vRM[0]);
	vRM.Add(-vRM[1]);
	vRM.Add(-vRM[2]);  
	FVector vDelta = TargetVector - Transform.GetLocation();
	vDelta.Normalize();
	TArray <int32> iSizes;
	for (FVector v : vRM)
	{
		int32 iSize = int32((v - vDelta).Size() * 10000);
		iSizes.Add(iSize);
	}
	TArray <int32> iSizesSorted = iSizes;
	iSizesSorted.Sort();
	int32 iFirst = iSizes.Find(iSizesSorted[0]);
	if (iFirst != INDEX_NONE)
	{
		if (iFirst == 0) FirstAxis = "X";
		if (iFirst == 1) FirstAxis = "Y";
		if (iFirst == 2) FirstAxis = "Z";
		if (iFirst == 3) FirstAxis = "-X";
		if (iFirst == 4) FirstAxis = "-Y";
		if (iFirst == 5) FirstAxis = "-Z";
	}
	int32 iSecond = iSizes.Find(iSizesSorted[1]);
	if (iSecond != INDEX_NONE)
	{
		if (iSecond == 0) SecondAxis = "X";
		if (iSecond == 1) SecondAxis = "Y";
		if (iSecond == 2) SecondAxis = "Z";
		if (iSecond == 3) SecondAxis = "-X";
		if (iSecond == 4) SecondAxis = "-Y";
		if (iSecond == 5) SecondAxis = "-Z";
	}
}

void UARigFunctions::GetAimAxis(FVector Aim, FVector Up, FTransform Transform, FAimSettings &AimSettings)
{
	AimSettings.AxisEnum = EAxisEnum::VE_XY;
	AimSettings.AimInvert = false;
	AimSettings.UpInvert = false;
	FString sFirstAxis, sSecondAxis, sCaseEqual;
	GetClosestAxis(Aim, Transform, sFirstAxis, sCaseEqual);
	GetClosestAxis(Up, Transform, sSecondAxis, sCaseEqual);
	if (sFirstAxis == sSecondAxis) sSecondAxis = sCaseEqual;
	if (sFirstAxis.MatchesWildcard("*X*") && sSecondAxis.MatchesWildcard("*Y*")) AimSettings.AxisEnum = EAxisEnum::VE_XY;
	if (sFirstAxis.MatchesWildcard("*X*") && sSecondAxis.MatchesWildcard("*Z*")) AimSettings.AxisEnum = EAxisEnum::VE_XZ;
	if (sFirstAxis.MatchesWildcard("*Y*") && sSecondAxis.MatchesWildcard("*X*")) AimSettings.AxisEnum = EAxisEnum::VE_YX;
	if (sFirstAxis.MatchesWildcard("*Y*") && sSecondAxis.MatchesWildcard("*Z*")) AimSettings.AxisEnum = EAxisEnum::VE_YZ;
	if (sFirstAxis.MatchesWildcard("*Z*") && sSecondAxis.MatchesWildcard("*X*")) AimSettings.AxisEnum = EAxisEnum::VE_ZX;
	if (sFirstAxis.MatchesWildcard("*Z*") && sSecondAxis.MatchesWildcard("*Y*")) AimSettings.AxisEnum = EAxisEnum::VE_ZY;
	if (sFirstAxis.MatchesWildcard("*-*")) AimSettings.AimInvert = true;
	if (sSecondAxis.MatchesWildcard("*-*")) AimSettings.UpInvert = true;
}

void UARigFunctions::GetAimSettings(FVector Aim, FVector Up, FTransform Transform, FAimSettings &AimSettings)
{
	AimSettings.Offset = FRotator::ZeroRotator;
	GetAimAxis(Aim, Up, Transform, AimSettings);
	FRotator rAimRot;
	AimConstraint(Aim, Up, Transform.GetLocation(), AimSettings, rAimRot);
	FRotator rOffset = FRotator(rAimRot.Quaternion().Inverse() * Transform.GetRotation());
	AimSettings.Offset = rOffset;
}

#undef LOCTEXT_NAMESPACE