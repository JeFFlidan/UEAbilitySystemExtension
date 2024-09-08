// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "MvPawnData.generated.h"

class APawn;
class UMvInputConfig;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// Class to instantiate for this pawn (should usually derive from AMvPawn or AMvCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MVAS|Pawn")
	TSubclassOf<APawn> PawnClass{nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MVAS|Input")
	TObjectPtr<UMvInputConfig> InputConfig{nullptr};
};
