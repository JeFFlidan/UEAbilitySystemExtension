// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Passive.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Passive)

UMvGameplayAbility_Passive::UMvGameplayAbility_Passive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActiveAdditionalGameplayEffectNum = 0;
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
		ActiveGameplayEffectHandles.Add(MvASC->ApplyGameplayEffectToSelf(
			GameplayEffect, EffectInfo.EffectLevel, MvASC->MakeEffectContext()));
	};

	if (ActiveGameplayEffectHandles.IsEmpty())
	{
		ApplyGameplayEffect(MainGameplayEffect);

		if (ActiveAdditionalGameplayEffectNum)
		{
			for (int32 i = 0; i != ActiveAdditionalGameplayEffectNum; ++i)
			{
				ApplyGameplayEffect(AdditionalGameplayEffects[i]);
			}
		}
	}
	else
	{
		if (ActiveAdditionalGameplayEffectNum >= AdditionalGameplayEffects.Num()
			|| AdditionalGameplayEffects.IsEmpty())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
			return;
		}
		
		float RandomNumber = FMath::RandRange(0.01f, 1.0f);
		if (RandomNumber <= AdditionalGameplayEffectDropChance)
		{
			UE_LOG(LogMvAbilitySystem, Display, TEXT("UMvGameplayAbility_Passive::ActivateAbility(): Sub effect will be activated."))
			ApplyGameplayEffect(AdditionalGameplayEffects[ActiveAdditionalGameplayEffectNum++]);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UMvGameplayAbility_Passive::RemoveAllActiveGameplayEffects()
{
	UMvAbilitySystemComponent* MvASC = GetMvAbilitySystemComponentFromActorInfo();
    check(MvASC);

	for (const FActiveGameplayEffectHandle& Handle : ActiveGameplayEffectHandles)
	{
		MvASC->RemoveActiveGameplayEffect(Handle);
	}

	ActiveGameplayEffectHandles.Reset();
}
