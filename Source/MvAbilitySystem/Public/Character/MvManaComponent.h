// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystem/Attributes/MvManaSet.h"

#include "Components/ActorComponent.h"
#include "MvManaComponent.generated.h"

class UMvAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMvOutOfMana, AActor*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMvManaChanged, UMvManaComponent*, ManaComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(Blueprintable, ClassGroup=(MvAbilitySystem), meta=(BlueprintSpawnableComponent))
class MVABILITYSYSTEM_API UMvManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "MVAS|Mana")
	FMvManaChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "MVAS|Mana")
	FMvManaChanged OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "MVAS|Mana")
	FMvOutOfMana OnOutOfMana;
	
	UMvManaComponent(const FObjectInitializer& ObjInitializer);

	UFUNCTION(BlueprintPure, Category = "MVAS|Mana")
	static UMvManaComponent* FindManaComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UMvManaComponent>() : nullptr;
	}
	
	void InitWithAbilitySystem(UMvAbilitySystemComponent* InASC);
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "MVAS|Mana")
	float GetMana() const { return ManaSet ? ManaSet->GetMana() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "MVAS|Mana")
	float GetManaNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Mana")
	float GetMaxMana() const { return ManaSet ? ManaSet->GetMaxMana() : 0.0f; }

protected:
	UPROPERTY()
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<const UMvManaSet> ManaSet;

	virtual void OnUnregister() override;

	void HandleManaChanged(
		AActor* Instigator,
		AActor* Causer,
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);
	
	void HandleMaxManaChanged(
		AActor* Instigator,
		AActor* Causer, 
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);

	void HandleOutOfMana(
		AActor* Instigator,
		AActor* Causer,
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);
};
