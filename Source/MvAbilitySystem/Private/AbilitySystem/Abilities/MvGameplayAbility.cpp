// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Character/MvCharacter.h"
#include "Character/MvHeroComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility)

UMvAbilitySystemComponent* UMvGameplayAbility::GetMvAbilitySystemComponentFromActorInfo() const
{
    return (CurrentActorInfo ? Cast <UMvAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

AController* UMvGameplayAbility::GetControllerFromActorInfo() const
{
    if (CurrentActorInfo)
    {
        if (AController* Controller = CurrentActorInfo->PlayerController.Get())
        {
            return Controller;
        }

        AActor* OwnerActor = CurrentActorInfo->OwnerActor.Get();
        while (OwnerActor)
        {
            if (AController* Controller = Cast<AController>(OwnerActor))
            {
                return Controller;
            }

            if (APawn* Pawn = Cast<APawn>(OwnerActor))
            {
                return Pawn->GetController();
            }

            OwnerActor = OwnerActor->GetOwner();
        }
    }

    return nullptr;
}

AMvCharacter* UMvGameplayAbility::GetMvCharacterFromActorInfo() const
{
    return (CurrentActorInfo ? Cast<AMvCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

UMvHeroComponent* UMvGameplayAbility::GetMvAbilityInputBindingComponent() const
{
    if (CurrentActorInfo)
    {
        return UMvHeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get());
    }
    return nullptr;
}

void UMvGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
    BP_OnAbilityAdded();
}

void UMvGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnRemoveAbility(ActorInfo, Spec);
    BP_OnAbilityRemoved();
}

void UMvGameplayAbility::OnPawnAvatarSet()
{
    BP_OnPawnAvatarSet();
}

void UMvGameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
    // TODO Needs message system
}
