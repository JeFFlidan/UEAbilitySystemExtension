// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Player/MvPlayerController.h"
#include "Player/MvPlayerState.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvPlayerController)

AMvPlayerController::AMvPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

AMvPlayerState* AMvPlayerController::GetMvPlayerState() const
{
	return Cast<AMvPlayerState>(PlayerState);
}

UMvAbilitySystemComponent* AMvPlayerController::GetMvAbilitySystemComponent() const
{
	const AMvPlayerState* MvPlayerState = GetMvPlayerState();
	return MvPlayerState ? MvPlayerState->GetMvAbilitySystemComponent() : nullptr;
}

void AMvPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UMvAbilitySystemComponent* AbilitySystemComponent = GetMvAbilitySystemComponent())
	{
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
