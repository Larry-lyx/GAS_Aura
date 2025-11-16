// Copyright GAS Aura Course


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	/**
	 * Case1 : Locally Controlled , get mouse cursor data and broadcast to GA
	 * Case2 : Not Locally Controlled , wait until get data from remote player and then broadcast to GA
	 */
	
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();

		// Bind call back to delegate that broadcast mouse data from Client to Server
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle , PredictionKey).AddUObject(this , &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle , PredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	/**
	 * 1. Get hit result under cursor from PC
	 * 2. Replicate hit result to Server
	 * 3. Broadcast mouse data to GA
	 */
	
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility , false , CursorHitResult);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHitResult;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle() ,
		GetActivationPredictionKey() ,
		DataHandle ,
		FGameplayTag() ,
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const
{
	/**
	 * 1. Consume received data , avoid to handle it many times
	 * 2. Broadcast reveived data (Mouse Data) to GA
	 */
	
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle() , GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
