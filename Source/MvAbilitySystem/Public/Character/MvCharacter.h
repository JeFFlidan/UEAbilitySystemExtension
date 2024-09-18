// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularCharacter.h"

#include "AbilitySystemInterface.h"
#include "MvCharacter.generated.h"

class UObject;
class UMvPawnExtensionComponent;
class UMvAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMvCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Reset() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMvAbilitySystemComponent* GetMvAbilitySystemComponent() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MVAS|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMvPawnExtensionComponent> PawnExtComponent;

	void UninitAndDestroy();

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();
};
