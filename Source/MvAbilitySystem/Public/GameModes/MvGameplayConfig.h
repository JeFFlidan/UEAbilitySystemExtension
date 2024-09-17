// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "MvGameplayConfig.generated.h"

class UGameFeatureAction;
class UMvPawnData;

/**
 * 
 */
UCLASS(BlueprintType, meta = (ShortToolTip = "Data asset used to define basic game features, pawn data and feature actions"))
class MVABILITYSYSTEM_API UMvGameplayConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TObjectPtr<UMvPawnData> PawnData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FString> GameFeatureToEnable;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
};
