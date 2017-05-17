#pragma once

#include "Engine.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "UnrealEd.h"
#include "ARigMenu.generated.h"

UCLASS(Blueprintable, Abstract)
class UARigMenu : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnToolTriggered(){};
	virtual void OnToolClosed(){};
};

