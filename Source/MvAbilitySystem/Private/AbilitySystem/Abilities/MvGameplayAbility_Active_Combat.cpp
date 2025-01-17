// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayAbility_Active_Combat)

UMvGameplayAbility_Active_Combat::UMvGameplayAbility_Active_Combat(const FObjectInitializer& ObjectInitializer)
	:  Super(ObjectInitializer)
{
	ComboIndex = 0;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMvGameplayAbility_Active_Combat::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	HitActors.Empty();
	
	if (ComboIndex >= Montages.Num())
	{
		ResetCombo();
	}

	const FMvAbilityMontageInfo& MontageInfo = Montages[ComboIndex++];

	// TODO Understand why it does not work stable
	// if (MontageInfo.Montage.IsValid())
	// {
	// 	CurrentAnimMontage = MontageInfo.Montage.Get();
	// 	PlayMontageWaitEvent(CurrentAnimMontage, MontageInfo.Speed);
	// }
	// else
	// {
	// 	UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvGameplayAbility_Active_Combat::ActivateAbility(): Montage is not loaded."))
	// }

	CurrentAnimMontage = MontageInfo.Montage.LoadSynchronous();
	PlayMontageWaitEvent(CurrentAnimMontage, MontageInfo.Speed);
}

void UMvGameplayAbility_Active_Combat::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::OnEventReceived(EventTag, EventData);
	check(EventData.Target);
	
	AActor* HitActor{EventData.Target};
	if (HitActors.Contains(HitActor))
	{
		return;
	}
	
	HitActors.AddUnique(HitActor);

	UMvAbilitySystemComponent* InstigatorASC = GetMvAbilitySystemComponent(EventData.Instigator);
	UMvAbilitySystemComponent* TargetASC = GetMvAbilitySystemComponent(EventData.Target);
	InstigatorASC->ApplyGameplayEffectToTarget(DamageEffectClass.GetDefaultObject(), TargetASC);

	if (EventData.InstigatorTags.IsValid())
	{
		FGameplayEventData EventData2 = EventData;
		EventData2.EventTag = EventData2.InstigatorTags.First();
		InstigatorASC->HandleGameplayEvent(EventData2.InstigatorTags.First(), &EventData2);
	}

	if (EventData.TargetTags.IsValid())
	{
		FGameplayEventData EventData2 = EventData;
		EventData2.EventTag = EventData2.TargetTags.First();
		TargetASC->HandleGameplayEvent(EventData2.TargetTags.First(), &EventData2);
	}
	
	CurrentActorInfo->GetAnimInstance()->Montage_Pause(CurrentAnimMontage);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ResetMontage, PauseHitMontage);
}

void UMvGameplayAbility_Active_Combat::ResetMontage() const
{
	CurrentActorInfo->GetAnimInstance()->Montage_Resume(CurrentAnimMontage);
}
