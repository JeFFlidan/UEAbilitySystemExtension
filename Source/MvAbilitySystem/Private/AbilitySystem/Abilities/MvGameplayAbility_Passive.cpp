// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Passive.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Passive)

UMvGameplayAbility_Passive::UMvGameplayAbility_Passive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AdditionalGameplayEffectDropChance = 0.01f;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMvGameplayAbility_Passive::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UMvAbilitySystemComponent* MvASC = GetMvAbilitySystemComponentFromActorInfo();
	check(MvASC);

	auto ApplyGameplayEffect = [&MvASC, this](const FMvAbilitySet_GameplayEffect& EffectInfo)
	{
		const UGameplayEffect* GameplayEffect = EffectInfo.GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		GrantedGameplayEffectHandles.Add(MvASC->ApplyGameplayEffectToSelf(
			GameplayEffect, EffectInfo.EffectLevel, MvASC->MakeEffectContext()));
	};
	
	if (TriggerEventData)
	{
		const UGameplayEffect* GameplayEffect = MainGameplayEffect.GameplayEffectClass->GetDefaultObject<UGameplayEffect>();

		switch (GameplayEffect->DurationPolicy)
		{
		case EGameplayEffectDurationType::Instant:
		case EGameplayEffectDurationType::HasDuration:
		{
			MvASC->ApplyGameplayEffectToSelf(GameplayEffect, MainGameplayEffect.EffectLevel, MvASC->MakeEffectContext());
			break;
		}
		case EGameplayEffectDurationType::Infinite:
			UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvGameplayAbility_Passive::ActivateAbility(): Gameplay Effect with an Infinite Duration Policy can't be applied by a gameplay event."))
			break;
		}
	}
	else
	{
		ApplyGameplayEffect(MainGameplayEffect);
	}

	for (const FMvAbilitySet_GameplayEffect& MvEffect : DraftedGameplayEffects)
	{
		ApplyGameplayEffect(MvEffect);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UMvGameplayAbility_Passive::DraftAdditionalGameplayEffect()
{
	if (DraftedGameplayEffects.Num() >= AdditionalGameplayEffects.Num())
	{
		return;
	}
	
	float RandomNumber = FMath::RandRange(0.01f, 1.0f);
	if (RandomNumber <= AdditionalGameplayEffectDropChance)
	{
		UE_LOG(LogMvAbilitySystem, Display, TEXT("UMvGameplayAbility_Passive::DraftAdditionalGameplayEffect(): Sub effect will be activated."))
		DraftedGameplayEffects.Add(AdditionalGameplayEffects[DraftedGameplayEffects.Num()]);
	}
}

void UMvGameplayAbility_Passive::RemoveGrantedGameplayEffects()
{
	UMvAbilitySystemComponent* MvASC = GetMvAbilitySystemComponentFromActorInfo();
    check(MvASC);

	for (const FActiveGameplayEffectHandle& Handle : GrantedGameplayEffectHandles)
	{
		MvASC->RemoveActiveGameplayEffect(Handle);
	}

	GrantedGameplayEffectHandles.Reset();
}

bool UMvGameplayAbility_Passive::IsTriggeredByGameplayEvent() const
{
	return !AbilityTriggers.IsEmpty();
}
