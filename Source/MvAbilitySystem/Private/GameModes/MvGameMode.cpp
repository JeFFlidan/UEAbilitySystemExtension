// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameModes/MvGameMode.h"
#include "GameModes/MvWorldSettings.h"
#include "GameModes/MvGameplayConfigManagerComponent.h"
#include "Character/MvPawnData.h"
#include "Character/MvPawnExtensionComponent.h"
#include "Player/MvPlayerState.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameMode)

AMvGameMode::AMvGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UMvPawnData* AMvGameMode::GetPawnData(const AMvPlayerState* MvPlayerState) const
{
	if (MvPlayerState)
	{
		if (const UMvPawnData* PawnData = MvPlayerState->GetPawnData())
		{
			return PawnData;
		}
	}

	UMvGameplayConfigManagerComponent* ConfigManager = GetGameplayConfigManager();

	if (ConfigManager->IsGameplayConfigLoaded())
	{
		const UMvGameplayConfig* GameplayConfig = ConfigManager->GetCurrentGameplayConfig();
		if (GameplayConfig->PawnData)
		{
			return GameplayConfig->PawnData;
		}
	}

	return nullptr;
}

void AMvGameMode::BeginPlay()
{
	Super::BeginPlay();

	LoadGameplayConfig();
}

void AMvGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMvGameMode::LoadGameplayConfig()
{
	if (AMvWorldSettings* WorldSettings = Cast<AMvWorldSettings>(GetWorldSettings()))
	{
		const TSoftClassPtr<UMvGameplayConfig>& GameplayConfig = WorldSettings->GetGameplayConfigClass();
		FString GameplayConfigSource = TEXT("WorldSettings");

		UE_LOG(LogMvAbilitySystem, Log, TEXT("Identified Gameplay Config %s (Source: %s)"),
			*GameplayConfig.ToString(), *GameplayConfigSource);
		
		GetGameplayConfigManager()->SetCurrentGameplayConfig(GameplayConfig);
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("AMvGameMode::LoadGameplayConfig(): Failed to get AMvWorldSettings. You must change WorldSettings class to AMvWorldSettings."))
	}
}

UMvGameplayConfigManagerComponent* AMvGameMode::GetGameplayConfigManager() const
{
	check(GameState);
	UMvGameplayConfigManagerComponent* ConfigManager = GameState->FindComponentByClass<UMvGameplayConfigManagerComponent>();
	check(ConfigManager);
	return ConfigManager;
}
