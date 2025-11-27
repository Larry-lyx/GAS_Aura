// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

/**
 * GA : Projectile Spell
 * general for all kinds of projectile spells
 *
 * main function:
 * Activate Ability
 * Spawn Projectile
 * Define GE
 *
 * Projectile GE is applied when the projectile actor hit the target
 * while the class of GE is defined in GA
 * so Projectile doesn't need to know what kind of damage it causes
 * Actually , Projectile Spell GA controls which GE will be applied
 */

class AAuraProjectile;

UCLASS()
class GAS_AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	/* Activate Ability Begin */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	/* Activate Ability End */

	/* Spawn Projectile Begin */
	
	// SpawnProjectile in Blueprint , because we want to spawn projectile when received notify from montage
	UFUNCTION(BlueprintCallable , Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation , const FGameplayTag& SocketTag , bool bOverridePitch = false , float PitchOverride = 0.f);
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	/* Spawn Projectile End */

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
