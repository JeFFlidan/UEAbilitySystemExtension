// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/AnimNotifies/AnimNotify_ComboWindowState.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MvGameplayAbility_Active_Combat.h"
#include "Character/MvCharacter.h"
#include "MvLogChannels.h"

UAnimNotify_ComboWindowState::UAnimNotify_ComboWindowState(const FObjectInitializer& ObjInitializer)
	:Super(ObjInitializer)
{
	bEndCombo = false;
}

void UAnimNotify_ComboWindowState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyBegin(): MeshComp is invalid."))
		return;
	}

	if (!MeshComp->GetOwner())
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyBegin(): MeshComp owner is invalid."))
		return;
	}
	
	if (AMvCharacter* Character = Cast<AMvCharacter>(MeshComp->GetOwner()))
	{
		AbilitySystemComponent = Character->GetMvAbilitySystemComponent();
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OpenComboWindow();
	}
}

void UAnimNotify_ComboWindowState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AbilitySystemComponent)
	{
		if (!AbilitySystemComponent->IsActiveNextCombo() && AbilitySystemComponent->IsLastComboMontage())
		{
			UE_LOG(LogMvAbilitySystem, Display, TEXT("UAnimNotify_ComboWindowState::NotifyEnd(): Reset combo."))
			AbilitySystemComponent->ResetCombo();
		}
		AbilitySystemComponent->CloseComboWindow();
	}
}

void UAnimNotify_ComboWindowState::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyTick(): ComboComponent is null."))
		return;
	}

	if (AbilitySystemComponent->IsOpenComboWindow() && AbilitySystemComponent->IsShouldTriggerCombo()
		&& AbilitySystemComponent->IsRequestTriggerCombo() && !AbilitySystemComponent->IsLastComboMontage())
	{
		if (AbilitySystemComponent->IsActiveNextCombo())
		{
			return;
		}
		
		const UMvGameplayAbility_Active_Combat* Ability = AbilitySystemComponent->GetActiveCombatAbility();
		if (!Ability)
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyTick(): Ability is null."))
			return;
		}
		
		if (AbilitySystemComponent->TryActivateAbilityByClass(Ability->GetClass()))
		{
			AbilitySystemComponent->ActivateNextCombo();
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyTick(): Failed to activate Ability [%s]"),
				*GetNameSafe(Ability));
		}
	}
}
