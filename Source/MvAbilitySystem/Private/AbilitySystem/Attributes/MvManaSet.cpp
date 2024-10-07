// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Attributes/MvManaSet.h"

#include "GameplayEffectExtension.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvManaSet)

UMvManaSet::UMvManaSet()
	: Mana(50.0f), MaxMana(50.0f), bOutOfMana(false)
{
}

void UMvManaSet::InitDelegates()
{
	Super::InitDelegates();

	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(GetManaAttribute()).AddUObject(this, &ThisClass::ManaCallaback);
		ASC->GetGameplayAttributeValueChangeDelegate(GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaCallback);
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvManaSet::InitDelegates(): Failed to bind delegates."))
	}
}

void UMvManaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute,NewValue);

	ClampAttribute(Attribute, NewValue);

	if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

void UMvManaSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute,NewValue);

	ClampAttribute(Attribute, NewValue);

	if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

void UMvManaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}

void UMvManaSet::ManaCallaback(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(nullptr, nullptr, nullptr, Data.NewValue - Data.OldValue, Data.OldValue, Data.NewValue);
}

void UMvManaSet::MaxManaCallback(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(nullptr, nullptr, nullptr, Data.NewValue - Data.OldValue, Data.OldValue, Data.NewValue);
}

void UMvManaSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
