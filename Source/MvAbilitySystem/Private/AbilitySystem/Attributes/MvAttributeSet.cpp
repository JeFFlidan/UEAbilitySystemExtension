// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Attributes/MvAttributeSet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"

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

void UMvAttributeSet::AdjustAttributeForMaxChange(
	const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute,
	float OldMaxValue,
	float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();

	if (!FMath::IsNearlyEqual(OldMaxValue, NewMaxValue) && AbilityComponent)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = OldMaxValue > 0.0f ? (CurrentValue * NewMaxValue / OldMaxValue) - CurrentValue : NewMaxValue;
		
		AbilityComponent->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
