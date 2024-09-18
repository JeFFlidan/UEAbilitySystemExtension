// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemComponent)

UMvAbilitySystemComponent::UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
}

UGameplayAbility* UMvAbilitySystemComponent::GetActiveComboAbility() const
{
	return nullptr;
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
	static TArray<FGameplayAbilitySpecHandle> AbilityToActivate;
	AbilityToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			const UMvGameplayAbility_Active* Ability = Cast<UMvGameplayAbility_Active>(AbilitySpec->Ability);
			
			if (Ability && Ability->GetActivationPolicy() == EMvAbilityActivationPolicy::WhileInputActive)
			{
				AbilityToActivate.AddUnique(AbilitySpec->Handle);
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
						AbilityToActivate.Add(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : AbilityToActivate)
	{
		TryActivateAbility(SpecHandle);
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
