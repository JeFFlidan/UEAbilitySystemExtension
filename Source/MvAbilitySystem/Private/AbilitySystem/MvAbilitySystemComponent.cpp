// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemComponent)

UMvAbilitySystemComponent::UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMvGameplayAbility_Active_Combat* UMvAbilitySystemComponent::GetActiveCombatAbility() const
{
	TArray<UGameplayAbility*> Abilities = GetActiveAbilitiesByClass(UMvGameplayAbility_Active_Combat::StaticClass());
	return Abilities.IsValidIndex(0) ? Cast<UMvGameplayAbility_Active_Combat>(Abilities[0]) : nullptr;
}

void UMvAbilitySystemComponent::CloseComboWindow()
{
	bWindowComboAttack = false;
	bRequestTriggerCombo = false;
	bNextComboAbilityActivated = false;
	bShouldTriggerCombo = false;
}

void UMvAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UMvAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UMvAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> NonCombatAbilitiesToActivate;
	static TArray<TSubclassOf<UMvGameplayAbility_Active_Combat>> CombatAbilitiesToActivate;
	NonCombatAbilitiesToActivate.Reset();
	CombatAbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			const UMvGameplayAbility_Active* Ability = Cast<UMvGameplayAbility_Active>(AbilitySpec->Ability);
			
			if (Ability && Ability->GetActivationPolicy() == EMvAbilityActivationPolicy::WhileInputActive)
			{
				NonCombatAbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UMvGameplayAbility_Active* Ability = Cast<UMvGameplayAbility_Active>(AbilitySpec->Ability);

					if (Ability && Ability->GetActivationPolicy() == EMvAbilityActivationPolicy::OnInputTriggered)
					{
						if (const UMvGameplayAbility_Active_Combat* CombatAbility = Cast<UMvGameplayAbility_Active_Combat>(Ability))
						{
							CombatAbilitiesToActivate.Add(CombatAbility->GetClass());
						}
						else
						{
							NonCombatAbilitiesToActivate.Add(AbilitySpec->Handle);
						}
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : NonCombatAbilitiesToActivate)
	{
		TryActivateAbility(SpecHandle);
	}

	for (TSubclassOf<UMvGameplayAbility_Active_Combat> CombatAbilityClass : CombatAbilitiesToActivate)
	{
		ActivateCombatAbility(CombatAbilityClass);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UMvAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Empty();
	InputReleasedSpecHandles.Empty();
	InputHeldSpecHandles.Empty();
}

bool UMvAbilitySystemComponent::IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilityClass)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::IsUsingAbilityByClass(): AbilityClass is null."))
		return false;
	}

	return GetActiveAbilitiesByClass(AbilityClass).Num() > 0;
}

TArray<UGameplayAbility*> UMvAbilitySystemComponent::GetActiveAbilitiesByClass(
	TSubclassOf<UGameplayAbility> AbilityToSearchClass) const
{
	const TArray<FGameplayAbilitySpec>& Specs = GetActivatableAbilities();
	TArray<UGameplayAbility*> ActiveAbilities;

	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.Ability && Spec.Ability.GetClass()->IsChildOf(AbilityToSearchClass))
		{
			TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();
			for (UGameplayAbility* AbilityInstance : AbilityInstances)
			{
				if (AbilityInstance->IsActive())
				{
					ActiveAbilities.Add(AbilityInstance);
				}
			}
		}
	}

	return ActiveAbilities;
}

void UMvAbilitySystemComponent::ActivateCombatAbility(TSubclassOf<UMvGameplayAbility_Active_Combat> CombatAbilityClass)
{
	bShouldTriggerCombo = false;

	if (IsUsingAbilityByClass(CombatAbilityClass))
	{
		bShouldTriggerCombo = bWindowComboAttack;
	}
	else
	{
		TryActivateAbilityByClass(CombatAbilityClass);
	}
}
