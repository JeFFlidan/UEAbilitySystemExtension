// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Input/MvInputConfig.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvInputConfig)

const UInputAction* UMvInputConfig::GetBasicInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	return GetInputActionByTag(BasicInputActions, InputTag, bLogNotFound);
}

const UInputAction* UMvInputConfig::GetConstantAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFount) const
{
	return GetInputActionByTag(ConstantAbilityInputActions, InputTag, bLogNotFount);
}

const UInputAction* UMvInputConfig::GetCustomizableAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFount) const
{
	return GetInputActionByTag(CustomizableAbilityInputActions, InputTag, bLogNotFount);
}


const UInputAction* UMvInputConfig::GetInputActionByTag(const TArray<FMvInputAction>& InputActions, const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMvInputAction& Action : InputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("Can't find InputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
