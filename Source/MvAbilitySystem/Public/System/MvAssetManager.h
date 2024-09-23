// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/AssetManager.h"
#include "MvAssetManager.generated.h"

struct FMvBundles
{
	inline static const FName GameplayCore{"GameplayCore"};
};

/**
 * This implementation of asset manager adds some functionality to handle asset loading for the MvAbilitySystem plugin.
 * It's recommended to override this class in your game if you use MvAbilitySystem plugin.
 * A custom AssetManager class can be used by setting 'AssetManagerClassName' in DefaultEngine.ini
 */
UCLASS(Config = Game)
class MVABILITYSYSTEM_API UMvAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMvAssetManager();

	static UMvAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPtr);

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPtr);

protected:
	static UObject* SyncLoadAsset(const FSoftObjectPath& AssetPath);
};

template<typename AssetType>
AssetType* UMvAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPtr)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPtr.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPtr.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SyncLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("UMvAssetManager::GetAsset(): Failed to load asset [%s]"),
				*AssetPtr.ToString());
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UMvAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPtr)
{
	TSubclassOf<AssetType> LoadedSubclass = nullptr;

	const FSoftObjectPath& AssetPath = AssetPtr.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPtr.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SyncLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("UMvAssetManager::GetSubclass(): Failed to load asset class [%s]."),
				*AssetPtr.ToString());
		}
	}

	return LoadedSubclass;
}
