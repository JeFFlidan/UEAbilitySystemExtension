// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/Attributes/MvHealthSet.h"
#include "MvLogChannels.h"

#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvHealthSet)

UMvHealthSet::UMvHealthSet()
	: Health(100.0f), MaxHealth(100.0f), bOutOfHealth(false)
{

}

void UMvHealthSet::InitDelegates()
{
	Super::InitDelegates();
	
	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute()).AddUObject(this, &ThisClass::HealthCallback);
		ASC->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthCallback);
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvHealthSet::InitDelegates(): Failed to bind delegates."))
	}
}

void UMvHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UMvHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

bool UMvHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	// TODO Add damage calculations
	return Super::PreGameplayEffectExecute(Data);
}

void UMvHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}

void UMvHealthSet::HealthCallback(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, Data.NewValue - Data.OldValue, Data.OldValue, Data.NewValue);
}

void UMvHealthSet::MaxHealthCallback(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, Data.NewValue - Data.OldValue, Data.OldValue, Data.NewValue);
}

void UMvHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
