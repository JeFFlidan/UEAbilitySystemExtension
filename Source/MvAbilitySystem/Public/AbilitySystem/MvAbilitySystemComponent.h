// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"

#include "MvAbilitySystemComponent.generated.h"

class UObject;
class UMvAttributeSet;
class UMvGameplayAbility_Active_Combat;
class UMvGameplayAbility_Passive;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category = "MVAS|Ability System", meta = (WorldContext = "WorldContextObject"))
	static UMvAbilitySystemComponent* FindPlayerMvAbilitySystemComponent(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "MVAS|Ability System")
	static UMvAbilitySystemComponent* FindMvAbilitySystemComponent(const AActor* Actor);
	
	FGameplayAbilitySpecHandle GrantAbility(const FGameplayAbilitySpec& AbilitySpec);

	// Must be used instead of GiveAbility
	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System", meta = (DisplayName = "Grant Ability"))
	FGameplayAbilitySpecHandle BP_GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	bool IsAbilityGranted(TSubclassOf<UGameplayAbility> AbilityClass) const;
	bool IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityClassToSearch) const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System|Input")
	void AddInputTagToAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System|Input")
	void RemoveInputTagFromAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System")
	void DeactivatePassiveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System")
	int32 GetGameplayEffectActivationCount(TSubclassOf<UGameplayEffect> GameplayEffectClass) const;
	
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(
		const FGameplayEffectSpec& GameplayEffect,
		FPredictionKey PredictionKey
	) override;

protected:
	UPROPERTY(EditAnywhere, Category = "MVAS|Ability System")
	float ComboResetTime;

	FGameplayAbilitySpec* LastActivatedCombatAbilitySpec;

	FTimerHandle ComboResetTimerHandle;

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	TMap<UClass*, int32> GameplayEffectActivationsCount;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void ActivateCombatAbility(FGameplayAbilitySpec* CombatAbilitySpec);

	UMvGameplayAbility_Active_Combat* GetCombatAbilityFromSpec(const FGameplayAbilitySpec* Spec) const;
	UMvGameplayAbility_Passive* GetPassiveAbilityFromSpec(const FGameplayAbilitySpec* Spec) const;
};
