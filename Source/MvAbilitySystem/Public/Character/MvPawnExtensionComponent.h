// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "MvPawnExtensionComponent.generated.h"

class UObject;
class UMvPawnData;
class UMvAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UMvPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "MVAS|Pawn")
	static UMvPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UMvPawnExtensionComponent>() : nullptr);
	}

	const UMvPawnData* GetPawnData() const { return PawnData; }
	void SetPawnData(const UMvPawnData* InPawnData);

	UFUNCTION(BlueprintPure, Category = "MVAS|Pawn")
	UMvAbilitySystemComponent* GetMvAbilitySystemComponent() const { return AbilitySystemComponent; }

	void InitializeAbilitySystem(UMvAbilitySystemComponent* InAbilitySystemComponent, AActor* InOwner);
	void UninitializeAbilitySystem();

	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	inline static const FName NAME_ActorFeatureName{"MvPawnExtension"};

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "MVAS|Pawn")
	TObjectPtr<const UMvPawnData> PawnData{nullptr};

	UPROPERTY(Transient)
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;

	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
