// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Aura Gameplay Tags
 *
 * Singleton: There’s only one instance in the whole project. 
 * - Static members are tied to the class itself; non-static members belong to this unique instance (not arbitrary objects).
 * - Access rules: Static members -> FAuraGameplayTags::MemberName; Instance members -> FAuraGameplayTags::Get().MemberName.
 * - Accessible throughout the project (the instance lives for the entire program lifecycle).
 *
 * Common steps to create a singleton:
 * 1. Declare a static instance in the private section (prevent external access).
 * 2. Define this unique instance in the .cpp file.
 * 3. Add a public static Getter (FAuraGameplayTags::Get()) for global access.
 *
 * Native Gameplay Tags:
 * - "Native" means defined in C++ (core tags, different from blueprint-defined tags).
 * - Custom Native Gameplay Tags must be added to the UE Engine (via AssetManager) by calling the initialization function.
 * - Role: Markers during gameplay—they indicate which properties we’re dealing with or what state an object is in.
 */

struct  FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;

	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TMap<FGameplayTag , FGameplayTag> DamageTypesToResistances;

	FGameplayTag Effects_HitReact;
	
protected:

private:
	static FAuraGameplayTags GameplayTags;
};
