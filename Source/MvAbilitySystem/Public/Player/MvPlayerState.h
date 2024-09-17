// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularPlayerState.h"
#include "GameModes/MvGameplayConfig.h"
#include "MvPlayerState.generated.h"

class UMvPawnData;
class UMvGameplayConfig;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	AMvPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	
	void SetPawnData(const UMvPawnData* InPawnData);
	const UMvPawnData* GetPawnData() const { return PawnData; }

protected:
	TObjectPtr<const UMvPawnData> PawnData;

private:
	void OnGameplayConfigLoaded(const UMvGameplayConfig* GameplayConfig);
};
