// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "MvAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MvManaSet.generated.h"

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvManaSet : public UMvAttributeSet
{
	GENERATED_BODY()

public:
	mutable FMvAttributeEvent OnManaChanged;
	mutable FMvAttributeEvent OnMaxManaChanged;
	mutable FMvAttributeEvent OnOutOfMana;
	
	UMvManaSet();

	ATTRIBUTE_ACCESSORS(UMvManaSet, Mana);
	ATTRIBUTE_ACCESSORS(UMvManaSet, MaxMana);

	virtual void InitDelegates() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Mana", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Mana", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxMana;

	bool bOutOfMana;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void ManaCallaback(const FOnAttributeChangeData& Data);
	void MaxManaCallback(const FOnAttributeChangeData& Data);
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
