// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

#include "MvAbilitySet.generated.h"

class UObject;
class UMvGameplayAbility;
class UMvAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UDataTable;

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
struct FMvAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FMvAbilitySet_AttributeSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> InitData;
};

USTRUCT(BlueprintType)
struct FMvAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void RemoveFromAbilitySystem(UMvAbilitySystemComponent* MvASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> AttributeSets;
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

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FMvAbilitySet_GameplayAbility> GameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<FMvAbilitySet_GameplayEffect> GameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets")
	TArray<FMvAbilitySet_AttributeSet> AttributeSets;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets")
	TObjectPtr<UDataTable> DefaultInitData;
};
