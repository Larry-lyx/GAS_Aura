// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraBaseCharacter , public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/* Highlight */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* /Highlight */
};
