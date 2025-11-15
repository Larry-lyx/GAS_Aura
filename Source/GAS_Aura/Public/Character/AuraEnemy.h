// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraBaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraBaseCharacter , public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/* Begin Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Enemy Interface */

	/* Begin Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
