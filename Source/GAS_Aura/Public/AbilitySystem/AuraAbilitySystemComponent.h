// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * Custom ASC based On UAbilitySystemComponent
 *
 * main functions added:
 * 1. When GE applied to Character , broadcast its containing gameplay tags
 * 2. Give , Activate and Release GA
 *
 * Basically , ASC hold every information we need in GAS
 * Almost all of the functions in GAS should be called through ASC
 */

class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags , const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityGiven , UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility , const FGameplayAbilitySpec&);

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilityGivenDelegate;

	bool bStartupAbilityGiven;

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);

	/* Give & Activate & Release Ability Begin */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	/* Give & Activate & Release Ability End */ 
	
protected:
	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client , Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent , const FGameplayEffectSpec& EffectSpec , FActiveGameplayEffectHandle ActiveEffectHandle);
};
