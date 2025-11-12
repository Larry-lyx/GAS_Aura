// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController =	nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController =	nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

UCLASS()
class GAS_AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* Begin Primary Attributes */
	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Strength , Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Strength);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Intelligence , Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Intelligence);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Resilience , Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Resilience);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Vigor , Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Vigor);
	/* End Primary Attributes */
	
	/* Begin Vital Attributes */
	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Health , Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Health);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_MaxHealth , Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , MaxHealth);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_Mana , Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , Mana);

	UPROPERTY(BlueprintReadOnly , ReplicatedUsing = Onrep_MaxMana , Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet , MaxMana);
	/* End Vital Attributes */

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data , FEffectProperties& Props) const;
};
