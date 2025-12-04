// Copyright GAS Aura Course


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ArcaneShards</>\n\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of: </><Damage>%d</>"
			"<Default> at the shard origin.</>") ,
			Level ,
			ManaCost ,
			Cooldown ,
			ScaledDamage 
			);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ArcaneShards</>\n\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: </><Damage>%d</>"
			"<Default> at the shard origins.</>") ,
			Level ,
			ManaCost ,
			Cooldown ,
			FMath::Min(MaxNumShards , Level),
			ScaledDamage
			);
	}
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: </><Damage>%d</>"
			"<Default> at the shard origins.</>") ,
			Level ,
			ManaCost ,
			Cooldown ,
			FMath::Min(MaxNumShards , Level),
			ScaledDamage
			);
}
