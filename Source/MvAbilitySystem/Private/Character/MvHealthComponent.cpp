// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvHealthComponent.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvHealthComponent)

UMvHealthComponent::UMvHealthComponent(const FObjectInitializer& ObjInitializer)
	: Super(ObjInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	DeathState = EMvDeathState::Alive;
}

void UMvHealthComponent::InitWithAbilitySystem(UMvAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvHealthComponent::InitWithAbilitySystem(): Health component for owner [%s] is initialized."),
			*GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvHealthComponent::InitWithAbilitySystem(): Cannot initialize health component for owner [%s] with null Ability System Component."),
			*GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UMvHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvHealthComponent::InitWithAbilitySystem(): Cannot initialize health component for owner [%s] with null health set."),
			*GetNameSafe(Owner));
		return;
	}

	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
}

void UMvHealthComponent::UninitializeFromAbilitySystem()
{
	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

void UMvHealthComponent::StartDeath()
{
	if (DeathState != EMvDeathState::Alive)
	{
		return;
	}

	DeathState = EMvDeathState::DeathStarted;

	AActor* Owner = GetOwner();
	check(Owner);
	OnDeathStarted.Broadcast(Owner);
}

void UMvHealthComponent::FinishDeath()
{
	if (DeathState != EMvDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EMvDeathState::DeathFinished;

	AActor* Owner = GetOwner();
	check(Owner);
	OnDeathFinished.Broadcast(Owner);
}

float UMvHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();
		return MaxHealth ? Health / MaxHealth : 0.0f;
	}
	
	return 0.0f;
}

void UMvHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UMvHealthComponent::HandleHealthChanged(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UMvHealthComponent::HandleMaxHealthChanged(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UMvHealthComponent::HandleOutOfHealth(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	// TODO
}
