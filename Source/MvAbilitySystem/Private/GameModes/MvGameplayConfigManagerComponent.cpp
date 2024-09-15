// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameModes/MvGameplayConfigManagerComponent.h"
#include "GameModes/MvGameplayConfig.h"
#include "MvLogChannels.h"

#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"

UMvGameplayConfigManagerComponent::UMvGameplayConfigManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMvGameplayConfigManagerComponent::SetCurrentGameplayConfig(const FPrimaryAssetId& GameplayConfigId)
{
	UAssetManager& AssetManager = UAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(GameplayConfigId);
	TSubclassOf<UMvGameplayConfig> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	const UMvGameplayConfig* GameplayConfig = GetDefault<UMvGameplayConfig>(AssetClass);

	check(GameplayConfig);
	check(!CurrentGameplayConfig);
	CurrentGameplayConfig = GameplayConfig;
	StartGameplayConfigLoad();
}

void UMvGameplayConfigManagerComponent::CallOrRegister_OnGameplayConfigLoaded(
	FOnMvGameplayConfigLoaded::FDelegate&& Delegate)
{
	if (IsGameplayConfigLoaded())
	{
		Delegate.Execute(CurrentGameplayConfig);
	}
	else
	{
		OnGameplayConfigLoaded.Add(MoveTemp(Delegate));
	}
}

const UMvGameplayConfig* UMvGameplayConfigManagerComponent::GetCurrentGameplayConfig() const
{
	check(LoadState == EMvGameplayConfigLoadState::Loaded);
	check(CurrentGameplayConfig);
	return CurrentGameplayConfig;
}

bool UMvGameplayConfigManagerComponent::IsGameplayConfigLoaded() const
{
	return LoadState == EMvGameplayConfigLoadState::Loaded && CurrentGameplayConfig;
}

void UMvGameplayConfigManagerComponent::StartGameplayConfigLoad()
{
	check(CurrentGameplayConfig);
	check(LoadState == EMvGameplayConfigLoadState::Unloaded);

	UE_LOG(LogMvAbilitySystem, Log, TEXT("Gameplay Config: StartGameplayConfigLoad(Current Config = %s)"),
		*CurrentGameplayConfig->GetPrimaryAssetId().ToString());

	LoadState = EMvGameplayConfigLoadState::Loading;
	
	// In the future, may be used as a complete delegate for StreamableHandle from AssetManager methods.
	OnGameplayConfigLoadComplete();
}

void UMvGameplayConfigManagerComponent::OnGameplayConfigLoadComplete()
{
	check(LoadState == EMvGameplayConfigLoadState::Loading);
	check(CurrentGameplayConfig);

	UE_LOG(LogMvAbilitySystem, Log, TEXT("Gameplay Config: OnGameplayConfigLoadComplete(Current Config = %s)"),
		*CurrentGameplayConfig->GetPrimaryAssetId().ToString());

	GameFeaturePluginURLs.Reset();

	UGameFeaturesSubsystem& GameFeaturesSubsystem = UGameFeaturesSubsystem::Get();
	
	for (const FString& PluginName : CurrentGameplayConfig->GameFeatureToEnable)
	{
		FString PluginURL;
		if (GameFeaturesSubsystem.GetPluginURLByName(PluginName, PluginURL))
		{
			GameFeaturePluginURLs.AddUnique(PluginURL);
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("OnGameplayConfigLoadComplete failed to find plugin URL from PluginName %s for Gameplay Config %s."),
				*PluginName, *CurrentGameplayConfig->GetPrimaryAssetId().ToString());
		}
	}

	GameFeaturePluginsLoadingCount = GameFeaturePluginURLs.Num();
	if (GameFeaturePluginsLoadingCount)
	{
		LoadState = EMvGameplayConfigLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			GameFeaturesSubsystem.LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnGameplayConfigFullLoadCompleted();
	}
}

void UMvGameplayConfigManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	--GameFeaturePluginsLoadingCount;

	if (GameFeaturePluginsLoadingCount == 0)
	{
		OnGameplayConfigFullLoadCompleted();
	}
}

void UMvGameplayConfigManagerComponent::OnGameplayConfigFullLoadCompleted()
{
	check(LoadState != EMvGameplayConfigLoadState::Loaded);
	check(CurrentGameplayConfig);

	LoadState = EMvGameplayConfigLoadState::ExecutingActions;

	FGameFeatureActivatingContext Context;
	if (const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld()))
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	for (UGameFeatureAction* Action : CurrentGameplayConfig->Actions)
	{
		if (Action)
		{
			Action->OnGameFeatureRegistering();
			Action->OnGameFeatureLoading();
			Action->OnGameFeatureActivating(Context);
		}
	}

	LoadState = EMvGameplayConfigLoadState::Loaded;
	OnGameplayConfigLoaded.Broadcast(CurrentGameplayConfig);
	OnGameplayConfigLoaded.Clear();
}

void UMvGameplayConfigManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameFeaturesSubsystem& GameFeaturesSubsystem = UGameFeaturesSubsystem::Get();
	
	for (const FString& PluginURL : GameFeaturePluginURLs)
	{
		GameFeaturesSubsystem.DeactivateGameFeaturePlugin(PluginURL);
	}

	if (LoadState == EMvGameplayConfigLoadState::Loaded)
	{
		LoadState = EMvGameplayConfigLoadState::Deactivating;

		NumExpectedPausers = INDEX_NONE;
		NumObservedPausers = 0;

		FGameFeatureDeactivatingContext Context(TEXT(""), [this](FStringView){ this->OnActionDeactivationCompleted(); });
		if (const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld()))
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}

		for (UGameFeatureAction* Action : CurrentGameplayConfig->Actions)
		{
			if (Action)
			{
				Action->OnGameFeatureDeactivating(Context);
				Action->OnGameFeatureUnregistering();
			}
		}

		NumExpectedPausers = Context.GetNumPausers();

		if (NumExpectedPausers > 0)
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("Failed to deactivating all actions from Gameplay Config %s"),
				*CurrentGameplayConfig->GetPrimaryAssetId().ToString())
		}

		// TODO make full unloading
		if (NumExpectedPausers == NumObservedPausers)
		{
			OnActionDeactivationCompleted();
		}
	}
}

void UMvGameplayConfigManagerComponent::OnActionDeactivationCompleted()
{
	check(IsInGameThread());
	++NumObservedPausers;

	if (NumObservedPausers == NumExpectedPausers)
	{
		OnAllActionsDeactivated();
	}
}

void UMvGameplayConfigManagerComponent::OnAllActionsDeactivated()
{
	LoadState = EMvGameplayConfigLoadState::Unloaded;
	CurrentGameplayConfig = nullptr;
}
