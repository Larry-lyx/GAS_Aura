// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * MMC is basically another way to calculate modifier magnitude from c++
 * we need to override CalculateBaseMagnitude_Implementation() to realize our own calculation
 *
 * MaxHealth = 80.f + 2.5f * Vigor + 10.f * PlayerLevel
 * so we need the value of Vigor and PlayerLevel
 * Vigor Value should be captured from AS
 * PlayerLevel can be derived from the creator of the GE
 */

UCLASS()
class GAS_AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
