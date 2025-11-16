// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/**
 * Ability Task is always called in GA Blueprint
 * so we need to make a static function for it
 *
 * It's kind of strange that UE told me : 0 blueprint usages
 * but actually it's used in several GA blueprints
 *
 * All we have to do is override Activate() , which is automatically called by GAS
 * however , I failed to find out where it's exactly called
 *
 * For Target Data Under Mouse
 * we should keep in mind that mouse data can only be accessed locally
 * and Server don't need to know where the mouse is in Client all the time
 * only when something important happened , like enabling GA by clicking somewhere , should Client replicate mouse data to Server
 *
 * To Replicate mouse data from Client to Server
 * we use a delegate which needs GetAbilitySpecHandle() and GetActivationPredictionKey()
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class GAS_AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable , Category = "Ability|Tasks" , meta = (DisplayName = "TargetDataUnderMouse" , HidePin = "OwningAbility" , DefaultToSelf = "OwningAbility" , BlueprintInternalUseOnly = "true") )
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	
	void SendMouseCursorData() const;
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle , FGameplayTag ActivationTag) const;
};
