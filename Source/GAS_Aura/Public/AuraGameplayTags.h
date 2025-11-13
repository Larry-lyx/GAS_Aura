// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Aura Gameplay Tags
 *
 * Singleton , there is only one in the whole project
 * contains native gameplay tags
 * native means defined in c++ , belongs to the core tags
 */
struct  FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
	
protected:

private:
	static FAuraGameplayTags GameplayTags;
};
