// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()


public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointChanged;
};
