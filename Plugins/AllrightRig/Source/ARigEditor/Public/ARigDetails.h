#pragma once

#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Private/DetailCategoryBuilderImpl.h"


class FARigDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static FReply ExecuteToolCommand(IDetailLayoutBuilder* DetailBuilder, UFunction* MethodToExecute);

};