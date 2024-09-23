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

private:
	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "MVAS|Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
};
