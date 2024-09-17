// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameModes/MvGameState.h"
#include "GameModes/MvGameplayConfigManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameState)

AMvGameState::AMvGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayConfigManagerComponent = CreateDefaultSubobject<UMvGameplayConfigManagerComponent>(TEXT("GameplayConfigManagerComponent"));
}
