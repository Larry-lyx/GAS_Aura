// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/*
 * Aura Asset Manager
 *
 * Custom Asset Manager for the Aura project, inherits from UE's UAssetManager.
 * Responsible for initializing core game systems/data when the project starts.
 *
 * In here, the main process is to Add custom native Gameplay Tags and Initialize GAS
 */

UCLASS()
class GAS_AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
