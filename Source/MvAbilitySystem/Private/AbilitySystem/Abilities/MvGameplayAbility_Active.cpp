// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Active.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Active)

UMvGameplayAbility_Active::UMvGameplayAbility_Active(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ActivationPolicy = EMvAbilityActivationPolicy::OnInputTriggered;
    ActivationGroup = EMvAbilityActivationGroup::Independent;

    bLogCancelation = false;
}

bool UMvGameplayAbility_Active::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        return false;
    }

    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // TODO Check activation group using Ability System Component

    return true;
}

void UMvGameplayAbility_Active::SetCanBeCanceled(bool bCanBeCanceled)
{
    if (!bCanBeCanceled && ActivationGroup == EMvAbilityActivationGroup::Dependent_Replaceable)
    {
        UE_LOG(LogMvAbilitySystem, Error, TEXT("SetCanBeCanceled: Ability [%s] can't block canceling because its activation group is Dependent_Replaceable."), *GetName());
        return;
    }

    Super::SetCanBeCanceled(bCanBeCanceled);
}

void UMvGameplayAbility_Active::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMvGameplayAbility_Active::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UMvGameplayAbility_Active::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
    {
        return false;
    }

    return true;
}

void UMvGameplayAbility_Active::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

FGameplayEffectContextHandle UMvGameplayAbility_Active::MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
    return Super::MakeEffectContext(Handle, ActorInfo);
}

void UMvGameplayAbility_Active::ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const
{
    Super::ApplyAbilityTagsToGameplayEffectSpec(Spec, AbilitySpec);
}

bool UMvGameplayAbility_Active::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    // TODO Replace with custom logic
    return Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, SourceTags, TargetTags, OptionalRelevantTags);
}
