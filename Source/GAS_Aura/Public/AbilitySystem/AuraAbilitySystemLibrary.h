// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * Custom Blueprint Function Library
 * all the functions here can be used in Blueprint and C++
 * so they must be static
 *
 * main functions:
 * 1. Get Overlay Widget Controller
 * 2. Get Attribute Menu Widget Controller
 * 3. Initialize Default Attributes for different character class (Enemy)
 */

class UOverlayWidgetController;

UCLASS()
class GAS_AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// pure means just get , doesn't change anything
	UFUNCTION(BlueprintPure , Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure , Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable , Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject , ECharacterClass CharacterClass , float Level , UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintCallable , Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject , UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintCallable , Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
};
