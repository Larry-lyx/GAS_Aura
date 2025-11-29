// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag CallbackTag , int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
