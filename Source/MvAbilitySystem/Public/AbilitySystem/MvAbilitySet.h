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
class UMvAttributeSet;
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
	TSubclassOf<UMvAttributeSet> AttributeSetClass;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bInitUsingCurveTables"))
	TObjectPtr<UDataTable> InitData;

	UPROPERTY(VisibleAnywhere)
	bool bInitUsingCurveTables{false};
};

USTRUCT(BlueprintType)
struct FMvAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UMvAttributeSet* Set);

	void RemoveFromAbilitySystem(UMvAbilitySystemComponent* MvASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UMvAttributeSet>> AttributeSets;
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
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FMvAbilitySet_GameplayAbility> GameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<FMvAbilitySet_GameplayEffect> GameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets")
	TArray<FMvAbilitySet_AttributeSet> AttributeSets;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (EditCondition = "!bInitUsingCurveTables"))
	TObjectPtr<UDataTable> DefaultInitData;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets")
	bool bInitUsingCurveTables{false};

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (EditCondition = "bInitUsingCurveTables"))
	FName GroupName;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (EditCondition = "bInitUsingCurveTables", ClampMin="1"))
	int32 Level{1};
};
