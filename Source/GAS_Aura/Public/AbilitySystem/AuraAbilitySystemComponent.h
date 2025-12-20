// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
class UAuraAbilitySystemComponent;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags , const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility , const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged , const FGameplayTag& /* AbilityTag */ , const FGameplayTag& /* Status Tag */ , int32 /* Ability Level */);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped , const FGameplayTag& /* AbilityTag */ , const FGameplayTag& /* Status */ , const FGameplayTag& /* Slot */ , const FGameplayTag& /* Previous Slot */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility , const FGameplayTag&  /* AbilityTag */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect , const FGameplayTag& /* AbilityTag */ , bool /* bActivate */);

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilityGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;

	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);

	bool bStartupAbilityGiven;

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec);

	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);

	bool IsSlotEmpty(const FGameplayTag& Slot);
	static bool AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec , const FGameplayTag& Slot);
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);

	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec , const FGameplayTag& Slot);
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	/**
	 * Multicast RPC implementation to trigger passive ability effects.
	 * Broadcasts a signal to activate/deactivate passive Niagara systems (project-specific behavior).
	 */
	UFUNCTION(NetMulticast , Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag , bool bActivate);

	/**
	 * Server-authoritative implementation for attribute upgrade.
	 * Sends a gameplay event (with magnitude 1) to trigger attribute modification
	 * (this is because Aura has GA_ListenForEvent, which can listen for event and use payload to make and apply GE to self),
	 * then deducts 1 attribute point from the avatar actor (via PlayerInterface).
	 */
	UFUNCTION(Server , Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatus(int32 Level);

	/**
	 * Server-authoritative RPC implementation to spend spell points and update ability status.
	 * Converts eligible abilities to unlocked status, levels up unlocked/equipped abilities,
	 * deducts 1 spell point (via CombatInterface), and notifies UI of ability changes.
	 */
	UFUNCTION(Server , Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	/**
	 * Server-authoritative RPC implementation to equip abilities (passive/offensive).
	 * Handles slot assignment logic: clears target slot (deactivates passive abilities in slot if needed),
	 * auto-activates new passive abilities, updates ability status to "Equipped", assigns slot to ability,
	 * marks spec as dirty, and notifies client of equip state.
	 */
	UFUNCTION(Server , Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag , const FGameplayTag& Slot);

	/**
	 * Client-side RPC implementation to broadcast ability equip state to UI.
	 * Triggers UI updates with ability's tag, status, assigned slot, and previous slot (if any).
	 */
	UFUNCTION(Client , Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag , const FGameplayTag& Status , const FGameplayTag& Slot , const FGameplayTag& PreviousSlot);

	static void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilityOfSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec , const FGameplayTag& Slot);

	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag , FString& OutDescription , FString& OutNextLevelDescription);
	
protected:
	virtual void OnRep_ActivateAbilities() override;

	/**
	 * Client-side callback for Gameplay Effect application to this ASC.
	 * Extracts all AssetTags from the applied GE and broadcasts them for UI/widget consumption.
	 */
	UFUNCTION(Client , Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent , const FGameplayEffectSpec& EffectSpec , FActiveGameplayEffectHandle ActiveEffectHandle);

	/**
	 * Client-side RPC implementation to notify UI widgets of ability status changes.
	 * Broadcasts updated ability state (tag, status, level) to client-side UI components.
	 */
	UFUNCTION(Client , Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag , int32 AbilityLevel);
};
