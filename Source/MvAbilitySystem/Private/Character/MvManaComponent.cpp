// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvManaComponent.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvManaComponent)

UMvManaComponent::UMvManaComponent(const FObjectInitializer& ObjInitializer)
	: Super(ObjInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UMvManaComponent::InitWithAbilitySystem(UMvAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvManaComponent::InitWithAbilitySystem(): Mana component for owner [%s] is initialized."),
			*GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvManaComponent::InitWithAbilitySystem(): Cannot initialize mana component for owner [%s] with null Ability System Component."),
			*GetNameSafe(Owner));
		return;
	}

	ManaSet = AbilitySystemComponent->GetSet<UMvManaSet>();
	if (!ManaSet)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvManaComponent::InitWithAbilitySystem(): Cannot initialize mana component for owner [%s] with null mana set."),
			*GetNameSafe(Owner));
		return;
	}

	ManaSet->OnManaChanged.AddUObject(this, &ThisClass::HandleManaChanged);
	ManaSet->OnMaxManaChanged.AddUObject(this, &ThisClass::HandleMaxManaChanged);
	ManaSet->OnOutOfMana.AddUObject(this, &ThisClass::HandleOutOfMana);
}

void UMvManaComponent::UninitializeFromAbilitySystem()
{
	if (ManaSet)
	{
		ManaSet->OnManaChanged.RemoveAll(this);
		ManaSet->OnMaxManaChanged.RemoveAll(this);
		ManaSet->OnOutOfMana.RemoveAll(this);
	}

	ManaSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UMvManaComponent::GetManaNormalized() const
{
	if (ManaSet)
	{
		const float Mana = ManaSet->GetMana();
		const float MaxMana = ManaSet->GetMaxMana();
		return MaxMana ? Mana / MaxMana : 0.0f;
	}
	
	return 0.0f;
}

void UMvManaComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UMvManaComponent::HandleManaChanged(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	OnManaChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UMvManaComponent::HandleMaxManaChanged(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	OnMaxManaChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UMvManaComponent::HandleOutOfMana(
	AActor* Instigator,
	AActor* Causer,
	const FGameplayEffectSpec* EffectSpec,
	float Magnitude,
	float OldValue,
	float NewValue)
{
	OnOutOfMana.Broadcast(GetOwner());
}
