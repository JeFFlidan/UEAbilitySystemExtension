// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Passive.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Passive)

#define LOCTEXT_NAMESPACE "MvGameplayAbility_Passive"

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
		const UGameplayEffect* MainEffect = GetGameplayEffectCDO(MainGameplayEffect);

		switch (MainEffect->DurationPolicy)
		{
		case EGameplayEffectDurationType::Instant:
		case EGameplayEffectDurationType::HasDuration:
		{
			MvASC->ApplyGameplayEffectToSelf(MainEffect, MainGameplayEffect.EffectLevel, MvASC->MakeEffectContext());
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

UGameplayEffect* UMvGameplayAbility_Passive::GetGameplayEffectCDO(const FMvAbilitySet_GameplayEffect& EffectInfo) const
{
	return EffectInfo.GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
}

#if WITH_EDITOR
EDataValidationResult UMvGameplayAbility_Passive::IsDataValid(FDataValidationContext& Context) const
{
	if (Super::IsDataValid(Context) == EDataValidationResult::Invalid)
	{
		return EDataValidationResult::Invalid;
	}
	
	bool bIsEventDriven = false;
	for (const FAbilityTriggerData& TriggerData : AbilityTriggers)
	{
		if (TriggerData.TriggerSource == EGameplayAbilityTriggerSource::GameplayEvent)
		{
			bIsEventDriven = true;
			break;
		}
	}

	UGameplayEffect* MainEffect = GetGameplayEffectCDO(MainGameplayEffect);
	
	if (bIsEventDriven)
	{
		if (MainEffect->DurationPolicy == EGameplayEffectDurationType::Infinite)
		{
			Context.AddError(FText::Format(LOCTEXT("InfiniteDurationInEventDrivenMainPassive",
				"Passive ability {0} is [Event Driven], so the main gameplay effect must not use ['Infinite' Duration Policy]."),
				FText::FromString(GetNameSafe(this))));

			return EDataValidationResult::Invalid;
		}
	}
	else
	{
		if (MainEffect->DurationPolicy != EGameplayEffectDurationType::Infinite)
		{
			Context.AddError(FText::Format(LOCTEXT("NoInfiniteDurationInDefaultMainPassive",
				"The [Non-Event-Driven] passive ability {0} attempts to use a main gameplay effect without ['Infinite' Duration Policy]."),
				FText::FromString(GetNameSafe(this))));

			return EDataValidationResult::Invalid;
		}
	}

	uint32 EntryIndex = 0;
	for (const FMvAbilitySet_GameplayEffect& Info : AdditionalGameplayEffects)
	{
		UGameplayEffect* Effect = GetGameplayEffectCDO(Info);
		if (Effect->DurationPolicy != EGameplayEffectDurationType::Infinite)
		{
			Context.AddError(FText::Format(LOCTEXT("NoInfiniteDurationAdditionalEffect",
				"Additional gameplay effect at index {0} does not use ['Infinite' Duration Policy]."), EntryIndex));
			return EDataValidationResult::Invalid;
		}

		++EntryIndex;
	}
	
	return EDataValidationResult::Valid;
}
#endif

#undef LOCTEXT_NAMESPACE
