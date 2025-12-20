// Copyright GAS Aura Course


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/LoadScreenSaveGame.h"
#include "GAS_Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

/**
 * Binds GE application delegate when ActorInfo is fully initialized.
 * Triggers ClientEffectApplied when a Gameplay Effect is applied to this ASC.
 */
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this , &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

/**
 * Iterates over all activatable abilities in this ASC and executes the provided delegate (with AbilitySpec).
 */
void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura , Error , TEXT("Failed to execute delegate in %hs") , __FUNCTION__);
		}
	}
}

/**
 * Get Ability Tag from provided Ability Spec
 */
FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec)
{
	if (GameplayAbilitySpec.Ability)
	{
		for (FGameplayTag Tag : GameplayAbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

/**
 * Get Input Tag from provided Ability Spec
 */
FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec)
{
	if (GameplayAbilitySpec.Ability)
	{
		for (FGameplayTag Tag : GameplayAbilitySpec.GetDynamicSpecSourceTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

/**
 * Get Status Tag from provided Ability Spec
 */
FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& GameplayAbilitySpec)
{
	if (GameplayAbilitySpec.Ability)
	{
		for (FGameplayTag Tag : GameplayAbilitySpec.GetDynamicSpecSourceTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

/**
 * Get Status Tag from Ability Tag
 */
FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

/**
 * Get Ability Tag from Ability Tag
 */
FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

/**
 * Checks if the specified input slot tag has no granted abilities bound to it.
 */
bool UAuraAbilitySystemComponent::IsSlotEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec , Slot))
		{
			return false;
		}
	}
	return true;
}

/**
 * Checks if the given AbilitySpec is bound to the specified input slot tag.
 */
bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec , const FGameplayTag& Slot)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

/**
 * Checks if the given AbilitySpec is bound to any input slot tag.
 */
bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

/**
 * Checks if the given AbilitySpec is passive ability
 */
bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag& AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag& AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive);
}

/**
 * Get AbilitySpec from the given ability tag
 */
FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

/**
 * Get the AbilitySpec which is bound to the given input slot tag
 */
FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

/**
 * Clear the old input slot tag of the provided AbilitySpec and Assign the newly given one.
 */
void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(Slot);
}

/**
 * Grants character abilities to this ASC from save game data.
 * Only activates equipped passive abilities (others are granted but inactive).
 * Marks startup abilities as granted and broadcasts AbilityGivenDelegate.
 */
void UAuraAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
	for (const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadAbilityClass = Data.GameplayAbility;

		FGameplayAbilitySpec LoadAbilitySpec = FGameplayAbilitySpec(LoadAbilityClass , Data.AbilityLevel);

		LoadAbilitySpec.GetDynamicSpecSourceTags().AddTag(Data.AbilitySlot);
		LoadAbilitySpec.GetDynamicSpecSourceTags().AddTag(Data.AbilityStatus);

		if (Data.AbilityType == FAuraGameplayTags::Get().Abilities_Type_Offensive)
		{
			GiveAbility(LoadAbilitySpec);
		}
		else if (Data.AbilityType == FAuraGameplayTags::Get().Abilities_Type_Passive)
		{
			if (Data.AbilityStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(LoadAbilitySpec);
			}
			else
			{
				GiveAbility(LoadAbilitySpec);
			}
		}
	}
	bStartupAbilityGiven = true;
	AbilityGivenDelegate.Broadcast();
}

/**
 * Grants character abilities to this ASC with the given StartupAbilities (offensive).
 * Only used when first load in the game.
 */
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass , 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilityGiven = true;
	AbilityGivenDelegate.Broadcast();
}

/**
 * Grants character abilities to this ASC with the given StartupAbilities (passive).
 */
void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass , 1);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

/**
 * Handles input tag press for bound Gameplay Abilities (GA).
 * Locates GA bound to the input tag, marks input as pressed, and replicates the press event to server if GA is active.
 * Note: Does not activate GA (activation logic is handled in AbilityInputTagHeld).
 */
void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
				FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
			
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed , AbilitySpec.Handle , OriginalPredictionKey);
			}
		}
	}
}

/**
 * Handles input tag hold for bound Gameplay Abilities (GA).
 * Locates GA bound to the input tag, marks input as pressed, and attempts to activate GA if inactive.
 * Triggers every frame while input is held (ETriggerEvent::Triggered) - minimal overhead (only iterates granted abilities).
 */
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

/**
 * Handles input tag release for bound Gameplay Abilities (GA).
 * Locates active GA bound to the input tag, marks input as released, and replicates the release event to server.
 * Note: Does not end GA (GA self-manages end timing); useful for player-initiated ability cancellation.
 */
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
			FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
			
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased , AbilitySpec.Handle , OriginalPredictionKey);
		}
	}
}

/**
 * Client-side entry point for attribute upgrade.
 * Checks if the avatar actor has available attribute points (via PlayerInterface),
 * and calls server-side upgrade logic if points are sufficient.
 */
void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor() , AttributeTag , Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor() , -1);
	}
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
                                                                                bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

/**
 * Updates ability status when the avatar's level changes.
 * Retrieves abilities from DA_AbilityInfo (abilities grantable to the avatar, 
 * not abilities currently managed by this ASC), grants abilities (meeting level requirements) to this ASC
 * and update status tag to Eligible, then notifies the client of the status change.
 */
void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability , 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag , FAuraGameplayTags::Get().Abilities_Status_Eligible , 1);
		}
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                           const FGameplayTag& StatusTag , int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag , AbilityLevel);
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor() , -1);
		}
		
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusTagFromSpec(*AbilitySpec);
		
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}

		ClientUpdateAbilityStatus(AbilityTag , Status , AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusTagFromSpec(*AbilitySpec);

		const bool bStatusValid = ( Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked));
		if (bStatusValid)
		{
			if (!IsSlotEmpty(Slot))
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if (SpecWithSlot)
				{
					// Is that Slot ability is the same as the new ability? If so, return early
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag , GameplayTags.Abilities_Status_Equipped , Slot , PrevSlot);
						return;
					}
				}
				if (IsPassiveAbility(*SpecWithSlot))
				{
					MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot) , false);
					DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
				}

				ClearSlot(SpecWithSlot);
			}

			if (!AbilityHasAnySlot(*AbilitySpec))
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag , true);
				}
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusTagFromSpec(*AbilitySpec));
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			
			AssignSlotToAbility(*AbilitySpec , Slot);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag , GameplayTags.Abilities_Status_Equipped , Slot , PrevSlot);
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
                                                                    const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag , Status , Slot , PreviousSlot);
}

/**
 * Removes the input slot tag associated with the given ability spec.
 */
void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->GetDynamicSpecSourceTags().RemoveTag(Slot);
}

/**
 * Clears all ability bindings to the specified input slot tag.
 * Iterates over activatable abilities and removes the target slot tag from any bound ability specs.
 */
void UAuraAbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec , Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

/**
 * Checks if the given ability spec is bound to the specified input slot tag.
 */
bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

/**
 * Retrieves ability descriptions (current/next level) by ability tag.
 * Populates OutDescription with the current level description (or locked description if ability is unavailable),
 * and OutNextLevelDescription with the next level's description (empty if max level/unavailable).
 */
bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
                                                              FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}

	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

/**
 * RepNotify handler for activated abilities replication.
 * Triggers the AbilityGivenDelegate broadcast once (on first activation) to confirm startup abilities are granted.
 * Ensures the delegate is only fired once via the bStartupAbilityGiven flag.
 */
void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilityGiven)
	{
		bStartupAbilityGiven = true;
		AbilityGivenDelegate.Broadcast();
	}
}

