// Copyright GAS Aura Course

#include "Character/AuraBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS_Aura/GAS_Aura.h"
#include "Kismet/GameplayStatics.h"

AAuraBaseCharacter::AAuraBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile , ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh() , FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;
}

UAbilitySystemComponent* AAuraBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraBaseCharacter::MultiCastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this , DeathSound , GetActorLocation() , GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None , true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic , ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse , NAME_None , true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;

	OnDeath.Broadcast(this);
}

int32 AAuraBaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraBaseCharacter::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraBaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered AAuraBaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& AAuraBaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

void AAuraBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraBaseCharacter::InitAbilityActorInfo()
{
}

UAnimMontage* AAuraBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraBaseCharacter::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld , true ));
	MultiCastHandleDeath(DeathImpulse);
}

FVector AAuraBaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTag(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

bool AAuraBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraBaseCharacter::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraBaseCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraBaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraBaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void AAuraBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass , Level ,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get() , GetAbilitySystemComponent());
}

void AAuraBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes , 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes , 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes , 1.f);
}

void AAuraBaseCharacter::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AAuraBaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(DissolveMaterialInstance , this);
		GetMesh()->SetMaterial(0 , DynamicMatIns);
		StartDissolveTimeline(DynamicMatIns);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance , this);
		Weapon->SetMaterial(0 , DynamicMatIns);
		StartWeaponDissolveTimeline(DynamicMatIns);
	}
}
