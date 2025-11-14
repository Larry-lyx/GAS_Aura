// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraBaseCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class GAS_AURA_API AAuraBaseCharacter : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraBaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	/* Begin Combat */
	UPROPERTY(EditAnywhere , Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere , Category = "Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;
	/* End Combat */

	/* Begin AbilitySystem */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	/* End AbilitySystem */

	/* Begin Initialize Default Attributes */
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass , float Level) const;
	void InitializeDefaultAttributes() const;
	/* End Initialize Default Attributes */

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};
