// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraConfigInput.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class GAS_AURA_API UAuraConfigInput : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag , bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
