// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

/**
 * Data Asset for Character Class
 * Three types of Character : Elementalist , Warrior , Ranger
 * each type has different PrimaryAttributes
 * while all of them share the same SecondaryAttributes and VitalAttributes
 *
 * Attributes here actually means the default values of different attributes
 * as we use GE to set their default value
 *
 * For Secondary Attributes , we use infinite GE , as primary attributes may change during the game and they should also change
 * the way to derive secondary attributes from primary attributes are the same
 *
 * For Vital Attributes , we simply use instant GE to set the initial value of health and mana
 */

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly , Category = "Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

UCLASS()
class GAS_AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly , Category = "Character Class Default")
	TMap<ECharacterClass , FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly , Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly , Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	FCharacterClassDefaultInfo GetClassDefaultInfo(const ECharacterClass CharacterClass);
};
