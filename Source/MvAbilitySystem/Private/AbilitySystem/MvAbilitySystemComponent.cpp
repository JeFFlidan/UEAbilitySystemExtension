// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MvAttributeSet.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Passive.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"
#include "Player/MvPlayerState.h"
#include "MvLogChannels.h"

#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemComponent)

UMvAbilitySystemComponent::UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ComboResetTime = 3.0f;
}

UMvAbilitySystemComponent* UMvAbilitySystemComponent::FindPlayerMvAbilitySystemComponent(const UObject* WorldContextObject)
{
	AMvPlayerState* PlayerState = CastChecked<AMvPlayerState>(UGameplayStatics::GetPlayerState(WorldContextObject, 0));

	return PlayerState->GetMvAbilitySystemComponent();
}

UMvAbilitySystemComponent* UMvAbilitySystemComponent::FindMvAbilitySystemComponent(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UMvAbilitySystemComponent>() : nullptr;
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
	static TArray<FGameplayAbilitySpec*> CombatAbilitiesToActivate;
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
						if (Cast<UMvGameplayAbility_Active_Combat>(Ability))
						{
							CombatAbilitiesToActivate.Add(AbilitySpec);
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

	for (FGameplayAbilitySpec* CombatAbilitySpec : CombatAbilitiesToActivate)
	{
		ActivateCombatAbility(CombatAbilitySpec);
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

void UMvAbilitySystemComponent::AddMvAttributeSet(UMvAttributeSet* AttributeSet)
{
	check(AttributeSet);
	AddAttributeSetSubobject(AttributeSet);
	AttributeSet->InitDelegates();
}

bool UMvAbilitySystemComponent::IsAbilityGranted(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	const TArray<FGameplayAbilitySpec>& Specs = GetActivatableAbilities();

	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.Ability && Spec.Ability.GetClass()->IsChildOf(AbilityClass))
		{
			return true;
		}
	}
	
	return false;
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
	TSubclassOf<UGameplayAbility> AbilityClassToSearch) const
{
	const TArray<FGameplayAbilitySpec>& Specs = GetActivatableAbilities();
	TArray<UGameplayAbility*> ActiveAbilities;

	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.Ability && Spec.Ability.GetClass()->IsChildOf(AbilityClassToSearch))
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

void UMvAbilitySystemComponent::AddInputTagToAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag)
{
	if (!AbilityClass->IsChildOf(UMvGameplayAbility_Active::StaticClass()))
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::AddInputTagToAbility(): Can't add InputTag to passive ability [%s]."),
			*GetNameSafe(AbilityClass));
		return;
	}

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass))
	{
		Spec->DynamicAbilityTags.AddTag(InputTag);
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::AddInputTagToAbility(): Ability [%s] is not granted."),
			*GetNameSafe(AbilityClass));
	}
}

void UMvAbilitySystemComponent::RemoveInputTagFromAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag)
{
	if (!AbilityClass->IsChildOf(UMvGameplayAbility_Active::StaticClass()))
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::RemoveInputTagFromAbility(): Can't remove InputTag from passive ability [%s]."),
			*GetNameSafe(AbilityClass));
		return;
	}

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass))
	{
		Spec->DynamicAbilityTags.RemoveTag(InputTag);
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::AddInputTagToAbility(): Ability [%s] is not granted."),
			*GetNameSafe(AbilityClass));
	}
}

void UMvAbilitySystemComponent::DeactivatePassiveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!AbilityClass->IsChildOf(UMvGameplayAbility_Passive::StaticClass()))
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::DeactivatePassiveAbilityByClass(): Can't deactivate active ability [%s]."),
					*GetNameSafe(AbilityClass));
		return;
	}

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(AbilityClass))
	{
		UMvGameplayAbility_Passive* PassiveAbility = CastChecked<UMvGameplayAbility_Passive>(Spec->GetPrimaryInstance());
		PassiveAbility->RemoveAllActiveGameplayEffects();
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAbilitySystemComponent::DeactivatePassiveAbilityByClass(): Ability [%s] is not granted."),
			*GetNameSafe(AbilityClass));
	}
}

void UMvAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
			Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UMvAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
			Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UMvAbilitySystemComponent::ActivateCombatAbility(FGameplayAbilitySpec* CombatAbilitySpec)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if (LastActivatedCombatAbilitySpec != CombatAbilitySpec)
	{
		if (LastActivatedCombatAbilitySpec)
		{
			GetCombatAbilityFromSpec(LastActivatedCombatAbilitySpec)->ResetCombo();
		}
		
		LastActivatedCombatAbilitySpec = CombatAbilitySpec;
		TimerManager.ClearTimer(ComboResetTimerHandle);
		TimerManager.SetTimer(ComboResetTimerHandle, FTimerDelegate::CreateLambda([this]
		{
			GetCombatAbilityFromSpec(this->LastActivatedCombatAbilitySpec)->ResetCombo();
			this->LastActivatedCombatAbilitySpec = nullptr;
		}), ComboResetTime, false);
	}

	TryActivateAbility(CombatAbilitySpec->Handle);
}

UMvGameplayAbility_Active_Combat* UMvAbilitySystemComponent::GetCombatAbilityFromSpec(FGameplayAbilitySpec* Spec) const
{
	return CastChecked<UMvGameplayAbility_Active_Combat>(Spec->GetPrimaryInstance());
}
