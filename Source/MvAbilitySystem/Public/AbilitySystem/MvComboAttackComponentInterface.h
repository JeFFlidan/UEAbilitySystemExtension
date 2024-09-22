// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "MvComboAttackComponentInterface.generated.h"

class UObject;
class UGameplayAbility;
class UMvGameplayAbility_Active_Combat;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMvComboAttackComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MVABILITYSYSTEM_API IMvComboAttackComponentInterface
{
	GENERATED_BODY()

public:
	virtual UMvGameplayAbility_Active_Combat* GetActiveCombatAbility() const = 0;
	virtual void IncrementComboIndex() = 0;
	virtual int32 GetComboIndex() const = 0;
	virtual void RequestTriggerCombo() = 0;
	virtual void ActivateNextCombo() = 0;
	virtual void ResetCombo() = 0;
	virtual void SetIsLastComboMontage(bool Value) = 0;
	virtual bool IsOpenComboWindow() const = 0;
	virtual bool IsActiveNextCombo() const = 0;
	virtual bool IsShouldTriggerCombo() const = 0;
	virtual bool IsRequestTriggerCombo() const = 0;
	virtual bool IsLastComboMontage() const = 0;
	virtual void OpenComboWindow() = 0;
	virtual void CloseComboWindow() = 0;
};
