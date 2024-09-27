// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MvAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class MVABILITYSYSTEM_API UMvAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	static UMvAbilitySystemGlobals& Get();
	virtual void InitGlobalData() override;

	inline static FName AttributeDefaultsOwnerName{"MvAbilitySystem"};

protected:
	virtual void ReloadAttributeDefaults() override;
};
