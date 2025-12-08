// Copyright GAS Aura Course


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;

	for (FLootItem& LootItem : LootItems)
	{
		for (int32 i = 0; i < LootItem.MaxNumToSpawn ; i++)
		{
			if (FMath::FRandRange(1.f , 100.f) < LootItem.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = LootItem.LootClass;
				NewItem.bLootLevelOverride = LootItem.bLootLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}

	return ReturnItems;
}
