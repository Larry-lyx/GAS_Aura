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

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGEClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const {return bIsBlockedHit;}
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}
	bool IsRadialDamage() const {return bIsRadialDamage;}

	float GetDebuffDamage() const {return DebuffDamage;}
	float GetDebuffFrequency() const {return DebuffFrequency;}
	float GetDebuffDuration() const {return DebuffDuration;}
	float GetRadialDamageInnerRadius() const {return RadialDamageInnerRadius;}
	float GetRadialDamageOuterRadius() const {return RadialDamageOuterRadius;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}
	FVector GetDeathImpulse() const {return DeathImpulse;}
	FVector GetKnockbackForce() const {return KnockbackForce;}
	FVector GetRadialDamageOrigin() const {return RadialDamageOrigin;}

	void SetBlockedHit(bool bInIsBlockedHit) {bIsBlockedHit = bInIsBlockedHit;}
	void SetCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetSuccessfulDebuff(bool bInIsSuccessfulDebuff){ bIsSuccessfulDebuff = bInIsSuccessfulDebuff;}
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage;}
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration;}
	void SetDebuffFrequency(float InDebuffFrequency){ DebuffFrequency = InDebuffFrequency;}
	void SetRadialDamageInnerRadius(float InInnerRadius){RadialDamageInnerRadius = InInnerRadius;}
	void SetRadialDamageOuterRadius(float InOuterRadius){RadialDamageOuterRadius = InOuterRadius;}
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	void SetRadialDamageOrigin(const FVector& InOrigin) { RadialDamageOrigin = InOrigin; }
	
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

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
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
