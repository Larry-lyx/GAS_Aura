// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * Base Class of Gameplay Ability (GA)
 *
 * GA is the commander of gameplay ability
 * it can spawn actor , define GE , bind to input ...
 *
 * each GA need to bind to input , so StartupInputTag is in the base class
 * other functions are realized in child class
 *
 * GA need to be granted to Character
 * after granted , we can access the Character using this GA
 * by using 'GetAvatarActorFromActorInfo()'
 */
UCLASS()
class GAS_AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly , Category = "Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Damage")
	FScalableFloat Damage;
};
