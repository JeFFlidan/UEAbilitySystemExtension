// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularGameMode.h"
#include "MvGameMode.generated.h"

class UMvPawnData;
class UMvGameplayConfig;
class AMvPlayerState;
class UMvGameplayConfigManagerComponent;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	AMvGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "MVAS|Pawn")
	const UMvPawnData* GetPawnData(const AMvPlayerState* MvPlayerState) const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void LoadGameplayConfig();

	UMvGameplayConfigManagerComponent* GetGameplayConfigManager() const;
};
