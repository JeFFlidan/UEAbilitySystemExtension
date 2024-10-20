// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"

#include "MvAbilitySystemComponent.generated.h"

class UObject;
class UMvAttributeSet;
class UMvGameplayAbility_Active_Combat;

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
	static UMvAbilitySystemComponent* FindMvAbilitySystemComponent(AActor* Actor);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	virtual void AddMvAttributeSet(UMvAttributeSet* AttributeSet);

	bool IsAbilityGranted(TSubclassOf<UGameplayAbility> AbilityClass) const;
	bool IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityClassToSearch) const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System|Input")
	void AddInputTagToAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System|Input")
	void RemoveInputTagFromAbility(TSubclassOf<UGameplayAbility> AbilityClass, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability System")
	void DeactivatePassiveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

protected:
	UPROPERTY(EditAnywhere, Category = "MVAS|Ability System")
	float ComboResetTime;

	FGameplayAbilitySpec* LastActivatedCombatAbilitySpec;

	FTimerHandle ComboResetTimerHandle;

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void ActivateCombatAbility(FGameplayAbilitySpec* CombatAbilitySpec);

	UMvGameplayAbility_Active_Combat* GetCombatAbilityFromSpec(FGameplayAbilitySpec* Spec) const;
};
