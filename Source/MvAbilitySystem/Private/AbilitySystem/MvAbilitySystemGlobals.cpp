// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemGlobals)

UMvAbilitySystemGlobals& UMvAbilitySystemGlobals::Get()
{
	return *CastChecked<UMvAbilitySystemGlobals>(IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals());
}

void UMvAbilitySystemGlobals::InitGlobalData()
{
	Super::InitGlobalData();
}
