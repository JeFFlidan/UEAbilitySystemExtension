// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Player/MvPlayerState.h"
#include "GameModes/MvGameMode.h"
#include "GameModes/MvGameplayConfigManagerComponent.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvPlayerState)

AMvPlayerState::AMvPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMvPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMvPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		UMvGameplayConfigManagerComponent* ConfigManager = GameState->FindComponentByClass<UMvGameplayConfigManagerComponent>();
		check(ConfigManager);

		ConfigManager->CallOrRegister_OnGameplayConfigLoaded(FOnMvGameplayConfigLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameplayConfigLoaded));
	}
}

void AMvPlayerState::SetPawnData(const UMvPawnData* InPawnData)
{
	PawnData = InPawnData;
}

void AMvPlayerState::OnGameplayConfigLoaded(const UMvGameplayConfig* GameplayConfig)
{
	if (AMvGameMode* MvGameMode = GetWorld()->GetAuthGameMode<AMvGameMode>())
	{
		if (const UMvPawnData* NewPawnData = MvGameMode->GetPawnData(this))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("AMvPlayerState::OnGameplayConfigLoaded(): Unable to find Pawn Data to init player state %s"),
				*GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("AMvPlayerState::OnGameplayConfigLoaded(): Failed to get Game Mode."))
	}
}
