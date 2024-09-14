// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "WorldActionBase_GameFeatureAction.h"
#include "UObject/SoftObjectPtr.h"

#include "AddInputContextMapping_GameFeatureAction.generated.h"

class UInputMappingContext;
struct FComponentRequestHandle;

USTRUCT()
struct FInputMappingContextInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
    TSoftObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditAnywhere, Category = "Input")
    int32 Priority{0};

    UPROPERTY(EditAnywhere, Category = "Input")
    bool bRegisterWithSettings{true};
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Mapping"))
class UAddInputContextMapping_GameFeatureAction final : public UWorldActionBase_GameFeatureAction
{
    GENERATED_BODY()

public:
    virtual void OnGameFeatureRegistering() override;
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
    virtual void OnGameFeatureUnregistering() override;

#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

private:
    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess="true"))
    TArray<FInputMappingContextInfo> InputMappings;

    TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
    TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;

    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

    void Reset();
    void HandleControllerExtension(AActor* InActor, FName EventName);
    void AddInputMappings(APlayerController* PlayerController);
    void RemoveInputMappings(APlayerController* PlayerController);
};