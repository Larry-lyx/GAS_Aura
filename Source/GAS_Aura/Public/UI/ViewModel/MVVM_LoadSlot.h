// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex , int32 , WidgetSwitcherIndex);

UCLASS()
class GAS_AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializeSlot();

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);

	FString GetLoadSlotName() const {return LoadSlotName;}
	FString GetPlayerName() const {return PlayerName;}
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , FieldNotify , Setter , Getter)
	FString PlayerName;


private:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , FieldNotify , Setter , Getter , meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;
};
