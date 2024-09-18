// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvCharacter.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Character/MvPawnExtensionComponent.h"

AMvCharacter::AMvCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnExtComponent = CreateDefaultSubobject<UMvPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(
		FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_RegisterAndCall(
		FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
}

void AMvCharacter::Reset()
{
	UninitAndDestroy();
	
	Super::Reset();
}

UAbilitySystemComponent* AMvCharacter::GetAbilitySystemComponent() const
{
	return GetMvAbilitySystemComponent();
}

UMvAbilitySystemComponent* AMvCharacter::GetMvAbilitySystemComponent() const
{
	if (PawnExtComponent)
	{
		return PawnExtComponent->GetMvAbilitySystemComponent();
	}
	return nullptr; 
}

void AMvCharacter::UninitAndDestroy()
{
	if (UMvAbilitySystemComponent* AbilitySystemComponent = GetMvAbilitySystemComponent())
	{
		if (AbilitySystemComponent->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}
}

void AMvCharacter::OnAbilitySystemInitialized()
{
}

void AMvCharacter::OnAbilitySystemUninitialized()
{
}
