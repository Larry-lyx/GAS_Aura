// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere , Category = "LootTiers|Spawning")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere , Category = "LootTiers|Spawning")
	int32 MaxNumToSpawn = 0;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "LootTiers|Spawning")
	bool bLootLevelOverride = true;
	
};

UCLASS()
class GAS_AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere , Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
	
};
