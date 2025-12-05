// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void SetLoadSlotName(FString InLoadSlotName);

	FString GetLoadSlotName() const {return LoadSlotName;}

private:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , FieldNotify , Setter , Getter , meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;
};
