// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "MvAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MvHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvHealthSet : public UMvAttributeSet
{
	GENERATED_BODY()

public:
	UMvHealthSet();
	
	ATTRIBUTE_ACCESSORS(UMvHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UMvHealthSet, MaxHealth);

	FMvAttributeEvent OnHealthChanged;
	FMvAttributeEvent OnMaxHealthChanged;
	FMvAttributeEvent OnOutOfDeath;

	virtual void InitDelegates() override;

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	bool bOutOfHealth;

	void HealthCallback(const FOnAttributeChangeData& Data);
	void MaxHealthCallback(const FOnAttributeChangeData& Data);
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
