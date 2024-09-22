// Copyright Kyrylo Zaverukha. All Rights Reserved.


#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"

#include "MvLogChannels.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

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

		if (MontageInfo.Montage.IsNull())
		{
			// Temp solution. Must be moved to bundle loading in the future.
			MontageInfo.Montage.ToSoftObjectPath().TryLoad();
		}

		CurrentAnimMontage = MontageInfo.Montage.Get();
		PlayMontageWaitEvent(MontageInfo.Montage.Get(), MontageInfo.Speed);
	}
}

void UMvGameplayAbility_Active_Combat::OnEventReceived(FGameplayTag EventTag, const FGameplayEventData& EventData)
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
