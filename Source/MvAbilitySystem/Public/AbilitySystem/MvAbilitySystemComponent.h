// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"

#include "MvAbilitySystemComponent.generated.h"

class UObject;
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

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	bool IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityToSearchClass) const;

protected:
	UPROPERTY(EditAnywhere, Category = "MVAS|Ability System")
	float ComboResetTime;

	FGameplayAbilitySpec* LastActivatedCombatAbilitySpec;

	FTimerHandle ComboResetTimerHandle;
	
	int32 ComboIndex;
	bool bWindowComboAttack;
	bool bRequestTriggerCombo;
	bool bNextComboAbilityActivated;
	bool bShouldTriggerCombo;
	bool bIsLastComboMontage;

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void ActivateCombatAbility(FGameplayAbilitySpec* CombatAbilitySpec);

	UMvGameplayAbility_Active_Combat* GetCombatAbilityFromSpec(FGameplayAbilitySpec* Spec) const;
};
