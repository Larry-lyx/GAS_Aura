// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraBaseCharacter.generated.h"

UCLASS(Abstract)
class GAS_AURA_API AAuraBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraBaseCharacter();

protected:
	virtual void BeginPlay() override;

};
