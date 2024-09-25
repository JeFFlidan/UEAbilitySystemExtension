// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "MvAbilitySystemSettings.generated.h"

class UMvAbilitySystemGlobals;
class UCurveTable;

/**
 * 
 */
UCLASS(config = MvAbilitySystem, DefaultConfig, meta = (DisplayName = "Mv Ability System"))
class MVABILITYSYSTEM_API UMvAbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMvAbilitySystemSettings(const FObjectInitializer& Initializer);

	/**
	 * Collection of Curve Tables that Gameplay Ability System will use to initialize Attribute Set if it
	 * requests initialization with FAttributeSetInitterDiscreteLevels.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Default")
	TArray<TSoftObjectPtr<UCurveTable>> AttributeSetDefaultsCurveTables;

	TArray<FSoftObjectPath> GetSoftObjectPaths() const;
};
