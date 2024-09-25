// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "MvComboAttackComponentInterface.h"

#include "MvAbilitySystemComponent.generated.h"

class UObject;
class UMvGameplayAbility_Active_Combat;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvAbilitySystemComponent : public UAbilitySystemComponent, public IMvComboAttackComponentInterface
{
	GENERATED_BODY()
	
public:
	UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UMvGameplayAbility_Active_Combat* GetActiveCombatAbility() const override;

	virtual void IncrementComboIndex() override { ++ComboIndex; }
	virtual int32 GetComboIndex() const override { return ComboIndex; }
	virtual void RequestTriggerCombo() override { bRequestTriggerCombo = true; }
	virtual void ActivateNextCombo() override { bNextComboAbilityActivated = true; }
	virtual void ResetCombo() override { ComboIndex = 0; }
	virtual void SetIsLastComboMontage(bool Value) override { bIsLastComboMontage = Value; }
	virtual bool IsOpenComboWindow() const override { return bWindowComboAttack; }
	virtual bool IsActiveNextCombo() const override { return bNextComboAbilityActivated; }
	virtual bool IsShouldTriggerCombo() const override { return bShouldTriggerCombo; }
	virtual bool IsRequestTriggerCombo() const override { return bRequestTriggerCombo; }
	virtual bool IsLastComboMontage() const override { return bIsLastComboMontage; }
	virtual void OpenComboWindow() override;
	virtual void CloseComboWindow() override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	bool IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityToSearchClass) const;

protected:
	UPROPERTY(EditAnywhere, Category = "MVAS|Ability System")
	float ComboResetTime;

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

	void ActivateCombatAbility(TSubclassOf<UMvGameplayAbility_Active_Combat> CombatAbilityClass);
};
