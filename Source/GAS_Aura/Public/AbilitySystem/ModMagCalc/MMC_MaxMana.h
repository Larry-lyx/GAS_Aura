// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * MMC is basically another way to calculate modifier magnitude from c++
 * we need to override CalculateBaseMagnitude_Implementation() to realize our own calculation
 *
 * MaxMana = 50.f + 2.5f * Intelligence + 15.f * PlayerLevel
 * so we need the value of Intelligence and PlayerLevel
 * Intelligence Value should be captured from AS
 * PlayerLevel can be derived from the creator of the GE
 */

UCLASS()
class GAS_AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
