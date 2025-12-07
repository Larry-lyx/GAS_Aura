// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAS_AURA_API ISaveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent)
	bool ShouldLoadTransform();

	UFUNCTION(BlueprintCallable , BlueprintNativeEvent)
	void LoadActor();
};
