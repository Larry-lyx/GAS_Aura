// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class GAS_AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly , Category = "WidgetControleer")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly , Category = "WidgetControleer")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly , Category = "WidgetControleer")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly , Category = "WidgetControleer")
	TObjectPtr<UAttributeSet> AttributeSet;
};
