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
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */

	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	
private:
	virtual void InitAbilityActorInfo() override;
};
