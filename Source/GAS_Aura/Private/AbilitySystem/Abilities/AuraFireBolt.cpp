// Copyright GAS Aura Course


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </><Damage>%d</>"
			"<Default> fire damage with a chance to burn</>") ,
			Level ,
			ManaCost ,
			Cooldown ,
			ScaledDamage 
			);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </><Damage>%d</>"
			"<Default> fire damage with a chance to burn</>") ,
			Level ,
			ManaCost ,
			Cooldown ,
			FMath::Min(NumProjectiles , Level),
			ScaledDamage
			);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"
		"<Small>Level:</><Level>%d</>\n"
		"<Small>Mana Cost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n\n"
		"<Default>Launches %d bolts of fire, "
		"exploding on impact and dealing: </><Damage>%d</>"
		"<Default> fire damage with a chance to burn</>") ,
		Level ,
		ManaCost ,
		Cooldown ,
		FMath::Min(NumProjectiles , Level),
		ScaledDamage
		);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo() ,
		SocketTag
		);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(- ProjectileSpread / 2.f , FVector::UpVector);
	
	// NumProjectiles = FMath::Min(MaxNumProjectile , GetAbilityLevel());
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo() , SocketLocation , SocketLocation + Direction * 75.f , 2 , FColor::Red , 120.f , 2.f );
		}
	}
	else
	{
		
	}


	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo() , SocketLocation , SocketLocation + Forward * 100.f , 2 , FColor::White , 120.f , 2.f );
}
