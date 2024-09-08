// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "MvPawnExtensionComponent.generated.h"

class UMvPawnData;
class UObject;

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

	inline static const FName NAME_ActorFeatureName{"MvPawnExtension"};

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "MVAS|Pawn")
	TObjectPtr<const UMvPawnData> PawnData{nullptr};

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
