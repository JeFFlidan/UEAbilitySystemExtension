// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularGameState.h"
#include "MvGameState.generated.h"

class UMvGameplayConfigManagerComponent;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	AMvGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY()
	TObjectPtr<UMvGameplayConfigManagerComponent> GameplayConfigManagerComponent;
};
