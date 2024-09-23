// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Active_Combat)

UMvGameplayAbility_Active_Combat::UMvGameplayAbility_Active_Combat(const FObjectInitializer& ObjectInitializer)
	:  Super(ObjectInitializer)
{
	
}

void UMvGameplayAbility_Active_Combat::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if (UMvAbilitySystemComponent* AbilitySystemComponent = GetMvAbilitySystemComponentFromActorInfo())
	{
		int32 ComboIndex = AbilitySystemComponent->GetComboIndex();
		AbilitySystemComponent->IncrementComboIndex();
		
		if (ComboIndex >= Montages.Num())
		{
			AbilitySystemComponent->ResetCombo();
			ComboIndex = AbilitySystemComponent->GetComboIndex();
		}
		else
		{
			if (ComboIndex == Montages.Num() - 1)
			{
				AbilitySystemComponent->SetIsLastComboMontage(true);
			}
			else
			{
				AbilitySystemComponent->SetIsLastComboMontage(false);
			}
		}
		
		const FMvAbilityMontageInfo& MontageInfo = Montages[ComboIndex];

		if (MontageInfo.Montage.IsValid())
		{
			CurrentAnimMontage = MontageInfo.Montage.Get();
			PlayMontageWaitEvent(CurrentAnimMontage, MontageInfo.Speed);
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvGameplayAbility_Active_Combat::ActivateAbility(): Montage is not loaded."))
		}
	}
}

void UMvGameplayAbility_Active_Combat::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);

	AActor* HitActor{EventData.Target};
	if (HitActors.Contains(HitActor))
	{
		return;
	}

	HitActors.AddUnique(HitActor);

	CurrentActorInfo->AnimInstance.Get()->Montage_Pause(CurrentAnimMontage);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ResetMontage, PauseHitMontage);
	(void)ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EventData.TargetData, DamageEffectClass, 1);
}

void UMvGameplayAbility_Active_Combat::ResetMontage() const
{
	CurrentActorInfo->AnimInstance.Get()->Montage_Resume(CurrentAnimMontage);
}
