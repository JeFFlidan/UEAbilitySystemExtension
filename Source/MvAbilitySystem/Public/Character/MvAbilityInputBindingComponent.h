// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "GameFeatures/AddInputContextMapping_GameFeatureAction.h"

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"

#include "MvAbilityInputBindingComponent.generated.h"

class MvInputConfig;
class UMvInputComponent;

/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MVABILITYSYSTEM_API UMvAbilityInputBindingComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UMvAbilityInputBindingComponent(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure, Category="MVAS|Input Binding")
	static UMvAbilityInputBindingComponent* FindAbilityBindingComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass< UMvAbilityInputBindingComponent>() : nullptr);
	}

	UMvInputComponent* GetMvInputComponent() const { return InputComponent; }

	void AddAdditionalInputConfig(MvInputConfig* InputConfig);
	void RemoveAdditionalInputConfig(MvInputConfig* InputConfig);
	bool IsReadyToBindInputs() const;

	inline static const FName NAME_BindInputsNow{"BindInputsNow"};
	inline static const FName NAME_ActorFeatureName{"InputBinding"};

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FInputMappingContextInfo> DefaultInputMappings;

	// Pointer to the input component that is cached for convenience
	UPROPERTY(Transient)
	TObjectPtr<UMvInputComponent> InputComponent;

	bool bReadyToBindInputs = false;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
};
