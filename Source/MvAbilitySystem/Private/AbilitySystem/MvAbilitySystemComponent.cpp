// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemComponent)

UMvAbilitySystemComponent::UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
}

UGameplayAbility* UMvAbilitySystemComponent::GetActiveComboAbility() const
{
	return nullptr;
}

void UMvAbilitySystemComponent::CloseComboWindow()
{
	bWindowComboAttack = false;
	bRequestTriggerCombo = false;
	bNextComboAbilityActivated = false;
	bShouldTriggerCombo = false;
}

void UMvAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	
}

void UMvAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	
}
