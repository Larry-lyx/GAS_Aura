// Copyright GAS Aura Course


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , Armor , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , ArmorPenetration , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , BlockChance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitChance , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitDamage , Source , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , CriticalHitResistance , Target , false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , FireResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , LightningResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , ArcaneResistance , Target , false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet , PhysicalResistance , Target , false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics  DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters , const TMap<FGameplayTag , FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	for (const auto& Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag&  DamageType = Pair.Key;
		const FGameplayTag&  DebuffType = Pair.Value;

		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType , false , -1.f);
		if (TypeDamage > -1.f)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance , false , -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag] , EvaluateParameters , TargetDebuffResistance);
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1 , 100) < EffectiveDebuffChance;

			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle , true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle , DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage , false , -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false , -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency , false , -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle , DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle , DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle , DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag , FGameplayEffectAttributeCaptureDefinition> TagToCaptureDef;
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	
	TagToCaptureDef.Add(Tags.Attributes_Secondary_Armor , DamageStatics().ArmorDef);
	TagToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration , DamageStatics().ArmorPenetrationDef);
	TagToCaptureDef.Add(Tags.Attributes_Secondary_BlockChance , DamageStatics().BlockChanceDef);
	TagToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitChance , DamageStatics().CriticalHitChanceDef);
	TagToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitDamage , DamageStatics().CriticalHitDamageDef);
	TagToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitResistance , DamageStatics().CriticalHitResistanceDef);
		
	TagToCaptureDef.Add(Tags.Attributes_Resistance_Fire , DamageStatics().FireResistanceDef);
	TagToCaptureDef.Add(Tags.Attributes_Resistance_Lightning , DamageStatics().LightningResistanceDef);
	TagToCaptureDef.Add(Tags.Attributes_Resistance_Arcane , DamageStatics().ArcaneResistanceDef);
	TagToCaptureDef.Add(Tags.Attributes_Resistance_Physical , DamageStatics().PhysicalResistanceDef);

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Damage = 0.f;

	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters , TagToCaptureDef);
	
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagToCaptureDef[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag , false);

		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef , EvaluateParameters , Resistance);
		Resistance = FMath::Clamp(Resistance , 0.f , 100.f);
		
		DamageTypeValue *= (100.f - Resistance) / 100.f;

		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. Override TakeDamage in BaseCharacter
			// 2. Create OnDamageDelegate, broadcast damage received in TakeDamage
			// 3. Bind lambda to OnDamageDelegate on the Victim here
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage
			//    (this will cause TakeDamage being called on the Victim, and broadcast delegate)
			// 5. In lambda, set the DamageTypeValue to the damage received from the OnDamageDelegate

			// there may be a little confusing, because we have to bind lambda first and then apply damage
			// so DamageTypeValue counted before(already calculated Resistance) is firstly used by ApplyRadialDamageWithFalloff
			// and being used to count the DamageAmount, so the radial damage is calculating after the resistance
			// after receiving the DamageAmount from the delegate, we override the DamageTypeValue
			
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageDelegate().AddLambda(
					[&](float DamageAmount)
					{
						DamageTypeValue = DamageAmount;
					}
					);
			}

			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar ,
				DamageTypeValue ,
				0.f ,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle) ,
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle) ,
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle) ,
				1.f ,
				UDamageType::StaticClass() ,
				TArray<AActor*>() ,
				SourceAvatar ,
				nullptr);
		}
		
		Damage += DamageTypeValue;
	}

	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef , EvaluateParameters , TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1 , 100) < TargetBlockChance;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle , bBlocked);
	
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef , EvaluateParameters , TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef , EvaluateParameters , SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration") , FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor") , FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef , EvaluateParameters , SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef , EvaluateParameters , SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef , EvaluateParameters , TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance") , FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(0 , 100) < EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle , bCriticalHit);
	
	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute() , EGameplayModOp::Additive , Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
