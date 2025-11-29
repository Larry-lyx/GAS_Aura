// Copyright GAS Aura Course

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/**
 * Aura Ability Types
 * 
 * FAuraGameplayEffectContext: Custom GE Context
 * - Adds hit type information: Blocked Hits and Critical Hits
 * 
 * Steps to create a Custom GE Context:
 * 1. Override GetScriptStruct() -> Tell Unreal Engine the structure of this custom GE Context
 * 2. Override Duplicate() -> Make a copy of this custom GE Context (required to create new instances)
 * 3. Override NetSerialize() -> Tell Unreal Engine how to replicate (sync over network) this custom GE Context
 */

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGEClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const {return bIsBlockedHit;}
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}

	float GetDebuffDamage() const {return DebuffDamage;}
	float GetDebuffFrequency() const {return DebuffFrequency;}
	float GetDebuffDuration() const {return DebuffDuration;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}

	void SetBlockedHit(bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}
	void SetCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetSuccessfulDebuff(bool bInIsSuccessfulDebuff){ bIsSuccessfulDebuff = bInIsSuccessfulDebuff;}
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage;}
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration;}
	void SetDebuffFrequency(float InDebuffFrequency){ DebuffFrequency = InDebuffFrequency;}
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		/**
		 * Creates a copy of this context, used to duplicate for later modifications
		 */
		
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};

/**
 * Tell UE the functions this struct supports
 */
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
