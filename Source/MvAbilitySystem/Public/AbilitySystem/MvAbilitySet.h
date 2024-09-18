// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "MvAbilitySet.generated.h"

class UObject;
class UMvGameplayAbility;
class UMvAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FMvAbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMvGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag AbilityTag;
};

USTRUCT(BlueprintType)
struct FMvAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	// TODO
};

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UMvAbilitySystemComponent* MvASC, FMvAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FMvAbilitySet_GameplayAbility> GameplayAbilities;
};
