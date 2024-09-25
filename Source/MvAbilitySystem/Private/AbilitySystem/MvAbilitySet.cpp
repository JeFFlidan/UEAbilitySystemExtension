// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MvGameplayAbility.h"
#include "AbilitySystem/Attributes/MvAttributeSet.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySet)

#define LOCTEXT_NAMESPACE "MvAbilitySet"

void FMvAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FMvAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FMvAbilitySet_GrantedHandles::AddAttributeSet(UMvAttributeSet* Set)
{
	if (Set)
	{
		AttributeSets.Add(Set);
	}
}

void FMvAbilitySet_GrantedHandles::RemoveFromAbilitySystem(UMvAbilitySystemComponent* MvASC)
{
	check(MvASC);

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			MvASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			MvASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : AttributeSets)
	{
		MvASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	AttributeSets.Reset();
}

void UMvAbilitySet::GiveToAbilitySystem(
	UMvAbilitySystemComponent* MvASC,
	FMvAbilitySet_GrantedHandles* OutGrantedHandles,
	UObject* SourceObject) const
{
	check(MvASC);

	for (const FMvAbilitySet_GameplayAbility& AbilityToGrant : GameplayAbilities)
	{
		UMvGameplayAbility* AbilityCDO = AbilityToGrant.AbilityClass->GetDefaultObject<UMvGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.AbilityTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = MvASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	for (const FMvAbilitySet_GameplayEffect& EffectToGrant : GameplayEffects)
	{
		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		FActiveGameplayEffectHandle EffectHandle = MvASC->ApplyGameplayEffectToSelf(
			GameplayEffect, EffectToGrant.EffectLevel, MvASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(EffectHandle);
		}
	}

	for (const FMvAbilitySet_AttributeSet& SetToGrant : AttributeSets)
	{
		UMvAttributeSet* AttributeSet = NewObject<UMvAttributeSet>(MvASC->GetOwner(), SetToGrant.AttributeSetClass);
		
		if (!bInitUsingCurveTables)
		{
			if (SetToGrant.InitData)
			{
				AttributeSet->InitFromMetaDataTable(SetToGrant.InitData);
			}
			else if (DefaultInitData)
			{
				AttributeSet->InitFromMetaDataTable(DefaultInitData);
			}
		}

		MvASC->AddAttributeSetSubobject(AttributeSet);

		if (bInitUsingCurveTables)
		{
			AttributeSet->InitFromCurveTables(GroupName, Level);
		}

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(AttributeSet);
		}
	}
}


#if WITH_EDITOR
void UMvAbilitySet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FProperty* Property = PropertyChangedEvent.Property;
	if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMvAbilitySet, bInitUsingCurveTables))
	{
		for (FMvAbilitySet_AttributeSet& Entry : AttributeSets)
		{
			Entry.bInitUsingCurveTables = bInitUsingCurveTables;
		}
	}
}

EDataValidationResult UMvAbilitySet::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const FMvAbilitySet_GameplayAbility& Entry : GameplayAbilities)
	{
		if (!Entry.AbilityClass)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("GameplayAbilityClassIsNull", "Empty AbilityClass at index {0} in GameplayAbilities."), EntryIndex));
		}
	}

	EntryIndex = 0;
	for (const FMvAbilitySet_GameplayEffect& Entry : GameplayEffects)
	{
		if (!Entry.GameplayEffectClass)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("GameplayEffectClassIsNull", "Empty GameplayEffectClass at index {0} in GameplayEffects."), EntryIndex));
		}
	}

	EntryIndex = 0;
	for (const FMvAbilitySet_AttributeSet& Entry : AttributeSets)
	{
		if (!Entry.AttributeSetClass)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("GameplayEffectClassIsNull", "Empty AttributeSetClass at index {0} in AttributeSets."), EntryIndex));
		}

		if (!bInitUsingCurveTables)
		{
			if (!Entry.InitData && !DefaultInitData)
			{
				Context.AddWarning(FText::Format(LOCTEXT("InitDataIsNull", "Empty InitData at index {0} in AttributeSets and missing DefaultInitData. It's recommended to use DataTable to init Attribute Set."), EntryIndex));
			}
		}
	}

	if (bInitUsingCurveTables)
	{
		if (GroupName.IsNone())
		{
			Context.AddWarning(LOCTEXT("GroupNameIsNone", "Attribute Set Group Name is invalid. You must configure a valid Group Name to init Attribute Sets using Curve Tables."));
		}
	}
	
	return Result;
}
#endif

#undef LOCTEXT_NAMESPACE