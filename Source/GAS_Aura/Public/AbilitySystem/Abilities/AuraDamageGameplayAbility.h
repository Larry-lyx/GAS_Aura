// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	/* Define GE Begin */
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	/* Define GE End */

	UPROPERTY(EditDefaultsOnly , Category = "Damage")
	TMap<FGameplayTag , FScalableFloat> DamageTypes;
};
