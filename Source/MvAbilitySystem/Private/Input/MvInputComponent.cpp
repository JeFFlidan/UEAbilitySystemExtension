// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Input/MvInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvInputComponent)

void UMvInputComponent::RemoveActionBindings(const FMvInputBindHandles& InputBindHandles)
{
	if (InputBindHandles.IsPressedBindHandleValid())
	{
		RemoveActionBindingForHandle(InputBindHandles.GetPressedBindHandle());
	}

	if (InputBindHandles.IsReleasedBindHandleValid())
	{
		RemoveActionBindingForHandle(InputBindHandles.GetReleasedBindHandle());
	}
}
