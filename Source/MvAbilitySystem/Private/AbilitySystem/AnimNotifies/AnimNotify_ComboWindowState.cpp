// Copyright Kyrylo Zaverukha. All Rights Reserved.


#include "AbilitySystem/AnimNotifies/AnimNotify_ComboWindowState.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Player/MvPlayerState.h"
#include "MvLogChannels.h"

#include "Kismet/GameplayStatics.h"
#include "Abilities/GameplayAbility.h"

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
	
	AbilitySystemComponent = GetMvAbilitySystemComponent();

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
		if (!AbilitySystemComponent->IsActiveNextCombo() && bEndCombo)
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
		&& AbilitySystemComponent->IsRequestTriggerCombo() && !bEndCombo)
	{
		if (AbilitySystemComponent->IsActiveNextCombo())
		{
			return;
		}
		
		const UGameplayAbility* Ability = AbilitySystemComponent->GetActiveComboAbility();
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

UMvAbilitySystemComponent* UAnimNotify_ComboWindowState::GetMvAbilitySystemComponent() const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	return PlayerController->GetPlayerState<AMvPlayerState>()->GetMvAbilitySystemComponent();
}
