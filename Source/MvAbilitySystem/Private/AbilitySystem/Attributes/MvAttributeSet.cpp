// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Attributes/MvAttributeSet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAttributeSet)

UMvAbilitySystemComponent* UMvAttributeSet::GetMvAbilitySystemComponent() const
{
	return Cast<UMvAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
