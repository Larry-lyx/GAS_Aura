// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraBaseCharacter , public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	/* Begin Init Ability Actor Info */
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/* End Init Ability Actor Info */

	/* Begin Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

	virtual void AddToXP_Implementation(int32 InXP) override;
	
private:
	virtual void InitAbilityActorInfo() override;
};
