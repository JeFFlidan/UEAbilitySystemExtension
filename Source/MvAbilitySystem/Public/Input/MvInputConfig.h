// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "MvInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FMvInputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Contains all basic input actions such as walking, crouching etc. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InptuAction"))
	TArray<FMvInputAction> BasicInputActions;

	/**
	 * Contains all input actions that have gameplay ability bindings that
	 * can't be customized by the player. When an action is triggered, the player's GAS component
	 * will try to activate the ability using the InputTag from the corresponding MvInputAction.
	 * An example would be a dash ability or a set of permanent active ability, that depends on
	 * the chosen character class, profession, etc.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FMvInputAction> ConstantAbilityInputActions;

	/**
	 * Contains all input actions that can activate gameplay abilities selected by the player.
	 * For example, you have some slots for active abilities, and the player can choose some skills
	 * from the list of ability. Each slot has an InputAction bound, and the InputAction must activate 
	 * ability that was placed in corresponding slot. After changing an ability in a slot, you must 
	 * remove all bindings from the corresponding InputAction and add new bindings with GameplayTags that 
	 * will be used to call the selected skill.
	 * InputTag from MvInputAction is used only for searching necessary InputAction.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FMvInputAction> CustomizableAbilityInputActions;

	UFUNCTION(BlueprintCallable)
	const UInputAction* GetBasicInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable)
	const UInputAction* GetConstantAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFount = true) const;

	UFUNCTION(BlueprintCallable)
	const UInputAction* GetCustomizableAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFount = true) const;

private:
	const UInputAction* GetInputActionByTag(const TArray<FMvInputAction>& InputActions, const FGameplayTag& InputTag, bool bLogNotFound) const;
};
