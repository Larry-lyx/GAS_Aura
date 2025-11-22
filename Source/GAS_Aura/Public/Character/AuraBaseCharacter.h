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

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

	UFUNCTION(NetMulticast , Reliable)
	virtual void MultiCastHandleDeath();

	UPROPERTY(EditAnywhere , Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	int32 MinionCount;
	virtual int32 GetMinionCount_Implementation() override;

protected:
	bool bDead = false;
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	/* Begin Combat */
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere , Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere , Category = "Combat")
	FName TailSocketName;

	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
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
	virtual void InitializeDefaultAttributes() const;
	/* End Initialize Default Attributes */

	void AddCharacterAbilities();

	/* Dissolve Effect */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	/* Dissolve Effect */

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Combat")
	UNiagaraSystem* BloodEffect;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Combat")
	USoundBase* DeathSound;

private:
	UPROPERTY(EditAnywhere , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere , Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
