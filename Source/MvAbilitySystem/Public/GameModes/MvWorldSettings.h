// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "MvWorldSettings.generated.h"

class UMvGameplayConfig;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

	const TSoftClassPtr<UMvGameplayConfig>& GetGameplayConfigClass() { return GameplayConfig; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSoftClassPtr<UMvGameplayConfig> GameplayConfig;
};
