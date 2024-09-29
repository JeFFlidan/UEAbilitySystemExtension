// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/AnimNotifies/AnimNotify_TriggerCombo.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Character/MvCharacter.h"
#include "MvLogChannels.h"

void UAnimNotify_TriggerCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//
	// if (AMvCharacter* Character = Cast<AMvCharacter>(MeshComp->GetOwner()))
	// {
	// 	Character->GetMvAbilitySystemComponent()->RequestTriggerCombo();
	// }
	// else
	// {
	// 	UE_LOG(LogMvAbilitySystem, Error, TEXT("UAnimNotify_TriggerCombo::Notify(): Failed to get AMvCharacter."))
	// }
}
