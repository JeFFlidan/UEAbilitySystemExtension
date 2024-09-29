// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/AnimNotifies/AnimNotify_ComboWindowState.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Character/MvCharacter.h"
#include "MvLogChannels.h"

UAnimNotify_ComboWindowState::UAnimNotify_ComboWindowState(const FObjectInitializer& ObjInitializer)
	:Super(ObjInitializer)
{
	
}

void UAnimNotify_ComboWindowState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//
	// if (!MeshComp)
	// {
	// 	UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyBegin(): MeshComp is invalid."))
	// 	return;
	// }
	//
	// if (!MeshComp->GetOwner())
	// {
	// 	UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_ComboWindowState::NotifyBegin(): MeshComp owner is invalid."))
	// 	return;
	// }
	//
	// if (AMvCharacter* Character = Cast<AMvCharacter>(MeshComp->GetOwner()))
	// {
	// 	AbilitySystemComponent = Character->GetMvAbilitySystemComponent();
	// }
	//
	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->OpenComboWindow();
	// }
}

void UAnimNotify_ComboWindowState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//
	// if (AbilitySystemComponent)
	// {
	// 	if (!AbilitySystemComponent->IsActiveNextCombo() && AbilitySystemComponent->IsLastComboMontage())
	// 	{
	// 		UE_LOG(LogMvAbilitySystem, Display, TEXT("UAnimNotify_ComboWindowState::NotifyEnd(): Reset combo."))
	// 		AbilitySystemComponent->ResetCombo();
	// 	}
	// 	AbilitySystemComponent->CloseComboWindow();
	// }
}
