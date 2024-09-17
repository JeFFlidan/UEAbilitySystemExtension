// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/GameStateComponent.h"
#include "MvGameplayConfigManagerComponent.generated.h"

class UMvGameplayConfig;

namespace UE::GameFeatures { struct FResult; }

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMvGameplayConfigLoaded, const UMvGameplayConfig*);

enum class EMvGameplayConfigLoadState : uint8
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

UCLASS()
class MVABILITYSYSTEM_API UMvGameplayConfigManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:	
	UMvGameplayConfigManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void SetCurrentGameplayConfig(const TSoftClassPtr<UMvGameplayConfig>& GameplayConfigClass);
	void CallOrRegister_OnGameplayConfigLoaded(FOnMvGameplayConfigLoaded::FDelegate&& Delegate);

	// Uses check to validate some info, so be careful when calling this method and choose the appropriate place.
	const UMvGameplayConfig* GetCurrentGameplayConfig() const;
	bool IsGameplayConfigLoaded() const;

private:
	TObjectPtr<const UMvGameplayConfig> CurrentGameplayConfig;
	EMvGameplayConfigLoadState LoadState = EMvGameplayConfigLoadState::Unloaded;

	int32 GameFeaturePluginsLoadingCount = 0;
	TArray<FString> GameFeaturePluginURLs;
	
	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	FOnMvGameplayConfigLoaded OnGameplayConfigLoaded;

	void StartGameplayConfigLoad();
	void OnGameplayConfigLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnGameplayConfigFullLoadCompleted();
	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();
};
