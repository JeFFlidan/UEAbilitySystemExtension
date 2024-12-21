// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvNPCStateComponent.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvNPCStateComponent)

UMvNPCStateComponent::UMvNPCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UMvAbilitySystemComponent>(TEXT("Ability System Component"));
}

UAbilitySystemComponent* UMvNPCStateComponent::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}
