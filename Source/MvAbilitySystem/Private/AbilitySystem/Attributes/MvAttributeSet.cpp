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
	float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComponent)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.0f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		
		AbilityComponent->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
