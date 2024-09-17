// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularPlayerState.h"
#include "GameModes/MvGameplayConfig.h"

#include "AbilitySystemInterface.h"

#include "MvPlayerState.generated.h"

class UMvPawnData;
class UMvGameplayConfig;
class UMvAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMvPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMvAbilitySystemComponent* GetMvAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	void SetPawnData(const UMvPawnData* InPawnData);
	const UMvPawnData* GetPawnData() const { return PawnData; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "MVAS|PlayerState")
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;
	
	TObjectPtr<const UMvPawnData> PawnData;

private:
	void OnGameplayConfigLoaded(const UMvGameplayConfig* GameplayConfig);
};
