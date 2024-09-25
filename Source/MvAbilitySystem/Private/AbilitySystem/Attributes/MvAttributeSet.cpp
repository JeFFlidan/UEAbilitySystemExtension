// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Attributes/MvAttributeSet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAttributeSet)

UMvAbilitySystemComponent* UMvAttributeSet::GetMvAbilitySystemComponent() const
{
	return Cast<UMvAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

void UMvAttributeSet::InitFromCurveTables(FName GroupName, int32 Level) const
{
	UAbilitySystemGlobals::Get().GetAttributeSetInitter()->InitAttributeSetDefaults(
		GetMvAbilitySystemComponent(), GroupName, Level, true);
}
