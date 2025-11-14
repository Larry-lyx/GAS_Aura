// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraConfigInput;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

struct FInputActionValue;

UCLASS()
class GAS_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();
	
	/* Begin Movement */
	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditAnywhere , Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere , Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	/* End Movement */

	/* Begin Cursor Trace */
	void CursorTrace();
	
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	/* End Cursor Trace */

	/* Begin Ability Input */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraConfigInput> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	/* End Ability Input */
};
