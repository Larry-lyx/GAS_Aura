// Copyright GAS Aura Course


#include "Input/AuraConfigInput.h"

const UInputAction* UAuraConfigInput::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTag == InputTag)
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp , Error , TEXT("Can't find AbilityInputAction for InputTag [%s] , on InputConfig [%s]") , *InputTag.ToString() , *GetNameSafe(this));
	}

	return nullptr;
}
