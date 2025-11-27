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
 * Almost all the functions in GAS should be called through ASC
 */

class UAuraAbilitySystemComponent;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags , const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility , const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged , const FGameplayTag& /* Ability Tag */ , const FGameplayTag& /* Status Tag */ , int32 /* Ability Level */);

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilityGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;

	bool bStartupAbilityGiven;

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	/* Give & Activate & Release Ability Begin */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	/* Give & Activate & Release Ability End */ 

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server , Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatus(int32 Level);

	UFUNCTION(Server , Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
protected:
	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client , Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent , const FGameplayEffectSpec& EffectSpec , FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client , Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag , int32 AbilityLevel);
};
