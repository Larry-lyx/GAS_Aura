// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Aura Gameplay Tags
 *
 * Singleton , there is only one instance
 * contains native gameplay tags
 * native means defined in c++ , belongs to the core tags
 */
struct  FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
protected:

private:
	static FAuraGameplayTags GameplayTags;
};
