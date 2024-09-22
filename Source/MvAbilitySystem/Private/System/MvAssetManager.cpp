// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "System/MvAssetManager.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAssetManager)

UMvAssetManager::UMvAssetManager()
{
}

UMvAssetManager* UMvAssetManager::Get()
{
	check(GEngine);

	if (UMvAssetManager* AssetManager = Cast<UMvAssetManager>(GEngine->AssetManager))
	{
		return AssetManager;
	}

	UE_LOG(LogMvAbilitySystem, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to UMvAssetManager or its subclass."))

	// Fatal error above prevents this from being called 
	return nullptr;
}

UObject* UMvAssetManager::SyncLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		if (IsInitialized())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, true);
		}

		return AssetPath.TryLoad();
	}

	UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvAssetManager::SyncLoadAsset(): AssetPath is null."))
	return nullptr;
}
