// Copyright GAS Aura Course


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadCastAbilityInfo();
	SpellPointChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag , const FGameplayTag& StatusTag , int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				
				bool bEnableSpendPointButton = false;
				bool bEnableEquipButton = false;
				ShouldEnabledButtons(StatusTag , CurrentSpellPoints , bEnableSpendPointButton , bEnableEquipButton);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton , bEnableEquipButton);
			}
			
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			bool bEnableSpendPointButton = false;
			bool bEnableEquipButton = false;
			ShouldEnabledButtons(SelectedAbility.Status , CurrentSpellPoints , bEnableSpendPointButton , bEnableEquipButton);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton , bEnableEquipButton);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagIsValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagIsValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	
	bool bEnableSpendPointButton = false;
	bool bEnableEquipButton = false;

	ShouldEnabledButtons(AbilityStatus , SpellPoints , bEnableSpendPointButton , bEnableEquipButton);

	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton , bEnableEquipButton);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::ShouldEnabledButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                      bool& bShouldEnabledSpendPointButton, bool& bShouldEnabledEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnabledEquipButton = false;
	bShouldEnabledSpendPointButton = false;

	if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
}
