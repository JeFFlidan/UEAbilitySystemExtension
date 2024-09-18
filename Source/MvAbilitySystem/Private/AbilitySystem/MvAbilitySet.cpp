// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MvGameplayAbility.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySet)

void UMvAbilitySet::GiveToAbilitySystem(
	UMvAbilitySystemComponent* MvASC,
	FMvAbilitySet_GrantedHandles* OutGrantedHandles,
	UObject* SourceObject) const
{
	check(MvASC);

	int32 EntryIndex = 0;

	for (const FMvAbilitySet_GameplayAbility& AbilityToGrant : GameplayAbilities)
	{
		if (!IsValid(AbilityToGrant.AbilityClass))
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("Ability with index %d in ability set %s is not valid"),
				EntryIndex, *GetNameSafe(this));
			continue;
		}

		UMvGameplayAbility* AbilityCDO = AbilityToGrant.AbilityClass->GetDefaultObject<UMvGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.AbilityTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = MvASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			// TODO add spec to granted handles
		}
	}
}
