#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimNode_SetBoneTransform.h"
#include "AnimGraphNode_SetBoneTransform.generated.h"


UCLASS()
class ARIGEDITOR_API UAnimGraphNode_SetBoneTransform : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
		FAnimNode_SetBoneTransform Node;

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FString GetNodeCategory() const override;
	virtual void CreateOutputPins() override;
};
