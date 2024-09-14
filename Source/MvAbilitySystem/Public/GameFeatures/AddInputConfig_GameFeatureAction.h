// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "WorldActionBase_GameFeatureAction.h"

#include "AddInputConfig_GameFeatureAction.generated.h"

class UMvInputConfig;

UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Config"))
class UAddInputConfig_GameFeatureAction final : public UWorldActionBase_GameFeatureAction
{
    GENERATED_BODY()

public:
    virtual void OnGameFeatureRegistering() override;
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
    virtual void OnGameFeatureUnregistering() override;

#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

private:
    struct FPerContextData
    {
        TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
        TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
    };
    
    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess="true"))
    TArray<TSoftObjectPtr<UMvInputConfig>> InputConfigs;

    TMap<FGameFeatureStateChangeContext, FPerContextData> Contexts;
    
    virtual void AddToWorld(
        const FWorldContext& WorldContext,
        const FGameFeatureStateChangeContext& ChangeContext) override;

    void Reset(FPerContextData& ActiveData);
    void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
    void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
    void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
};