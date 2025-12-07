// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraConfigInput;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

enum class ETargetingStatus : uint8
{
	TargetingEnemy ,
	TargetingNonEnemy ,
	NotTargeting
};

UCLASS()
class GAS_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client , Reliable)
	void ShowDamageNumber(float DamageAmount , ACharacter* TargetCharacter, bool bBlockedHit , bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	/* Begin Aura ASC */
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();
	/* End Aura ASC */
	
	/* Begin Movement */
	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditAnywhere , Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere , Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	/* End Movement */

	UPROPERTY(EditAnywhere , Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed(){bShiftKeyDown = true;}
	void ShiftReleased(){bShiftKeyDown = false;}
	bool bShiftKeyDown = false;

	/* Begin Cursor Trace */
	void CursorTrace();

	UPROPERTY()
	TObjectPtr<AActor> LastActor;
	UPROPERTY()
	TObjectPtr<AActor> ThisActor;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	FHitResult CursorHitResult;
	/* End Cursor Trace */

	/* Begin Ability Input */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraConfigInput> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	/* End Ability Input */

	/* Begin Click to Move */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressedThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();
	/* End Click to Move */

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircletLocation();
};
