// Copyright GAS Aura Course


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCparams)
{
	PlayerController = WCparams.PlayerController;
	PlayerState = WCparams.PlayerState;
	AbilitySystemComponent = WCparams.AbilitySystemComponent;
	AttributeSet = WCparams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
