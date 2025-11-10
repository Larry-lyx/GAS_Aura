// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBaseCharacter.h"
#include "AuraCharacter.generated.h"

UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraBaseCharacter
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	/* Begin Init Ability Actor Info */
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/* End Init Ability Actor Info */
	
private:
	virtual void InitAbilityActorInfo() override;
};
