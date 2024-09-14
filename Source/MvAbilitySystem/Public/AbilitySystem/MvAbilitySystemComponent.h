// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "MvComboAttackComponentInterface.h"

#include "MvAbilitySystemComponent.generated.h"

class UObject;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvAbilitySystemComponent : public UAbilitySystemComponent, public IMvComboAttackComponentInterface
{
	GENERATED_BODY()
	
public:
	UMvAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UGameplayAbility* GetActiveComboAbility() const;

	virtual void IncrementComboIndex() override { if (bWindowComboAttack) ++ComboIndex; }
	virtual int32 GetComboIndex() const override { return ComboIndex; }
	virtual void RequestTriggerCombo() override { bRequestTriggerCombo = true; }
	virtual void ActivateNextCombo() override { bNextComboAbilityActivated = true; }
	virtual void ResetCombo() override { ComboIndex = 0; }
	virtual bool IsOpenComboWindow() const override { return bWindowComboAttack; }
	virtual bool IsActiveNextCombo() const override { return bNextComboAbilityActivated; }
	virtual bool IsShouldTriggerCombo() const override { return bShouldTriggerCombo; }
	virtual bool IsRequestTriggerCombo() const override { return bRequestTriggerCombo; }
	virtual void OpenComboWindow() override { bWindowComboAttack = true; } 
	virtual void CloseComboWindow() override;

protected:
	UPROPERTY()
	int32 ComboIndex;
	
	UPROPERTY()
	bool bWindowComboAttack;

	UPROPERTY()
	bool bRequestTriggerCombo;

	UPROPERTY()
	bool bNextComboAbilityActivated;

	UPROPERTY()
	bool bShouldTriggerCombo;
};
