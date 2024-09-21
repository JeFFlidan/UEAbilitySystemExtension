// Copyright Kyrylo Zaverukha. All Rights Reserved.


#include "AbilitySystem/AnimNotifies/AnimNotify_TriggerCombo.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Player/MvPlayerState.h"

#include "Kismet/GameplayStatics.h"

void UAnimNotify_TriggerCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (AMvPlayerState* PlayerState = PlayerController->GetPlayerState<AMvPlayerState>())
	{
		PlayerState->GetMvAbilitySystemComponent()->RequestTriggerCombo();
	}
}
