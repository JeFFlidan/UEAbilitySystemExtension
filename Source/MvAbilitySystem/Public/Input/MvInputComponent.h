// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "EnhancedInputComponent.h"
#include "MvInputConfig.h"

#include "MvInputComponent.generated.h"

USTRUCT()
struct FMvInputBindHandles
{
	GENERATED_BODY()

public:
	FMvInputBindHandles() = default;

	FMvInputBindHandles(uint32 PressedHandle, uint32 ReleasedHandle)
		: PressedHandle(PressedHandle), ReleasedHandle(ReleasedHandle) { }

	bool IsPressedBindHandleValid() const { return PressedHandle != 0; }
	bool IsReleasedBindHandleValid() const { return ReleasedHandle != 0; }

	uint32 GetPressedBindHandle() const { return PressedHandle; }
	uint32 GetReleasedBindHandle() const { return ReleasedHandle; }

private:
	uint32 PressedHandle{ 0 };
	uint32 ReleasedHandle{ 0 };
};

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename FuncType>
	void BindBasicAction(const UMvInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindConstantAbilityActions(const UMvInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	FMvInputBindHandles BindCustomizableAbilityAction(const UMvInputConfig* InputConfig, const FGameplayTag& InputTag, const FGameplayTag& AbilityTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

	void RemoveActionBindings(const FMvInputBindHandles& InputBindHandles);
};

template<class UserClass, typename FuncType>
void UMvInputComponent::BindBasicAction(const UMvInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* InputAction = InputConfig->GetBasicInputActionByTag(InputTag))
	{
		if (Func)
		{
			BindAction(InputAction, ETriggerEvent::Started, Object, Func, InputTag);
		}
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMvInputComponent::BindConstantAbilityActions(const UMvInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FMvInputAction& Action : InputConfig->ConstantAbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag).GetHandle();
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle();
			}
		}
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
FMvInputBindHandles UMvInputComponent::BindCustomizableAbilityAction(const UMvInputConfig* InputConfig, const FGameplayTag& InputTag, const FGameplayTag& AbilityTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	uint32 PressedHandle = 0, ReleasedHandle = 0;
	if (const UInputAction* InputAction = InputConfig->GetCustomizableAbilityInputActionByTag(InputTag))
	{
		if (PressedFunc)
		{
			PressedHandle = BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, AbilityTag).GetHandle();
		}

		if (ReleasedFunc)
		{
			ReleasedHandle = BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityTag).GetHandle();
		}
	}

	return FMvInputBindHandles(PressedHandle, ReleasedHandle);
}
