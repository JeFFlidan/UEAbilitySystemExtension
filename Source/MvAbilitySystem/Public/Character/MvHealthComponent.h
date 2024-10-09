// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystem/Attributes/MvHealthSet.h"

#include "Components/ActorComponent.h"
#include "MvHealthComponent.generated.h"

class UMvAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMvHealthDeathEvent, AActor*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMvHealthChanged, UMvHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UENUM(BlueprintType)
enum class EMvDeathState : uint8
{
	Alive,
	DeathStarted,
	DeathFinished
};

UCLASS(Blueprintable, ClassGroup=(MvAbilitySystem), meta=(BlueprintSpawnableComponent))
class MVABILITYSYSTEM_API UMvHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "MVAS|Health")
	FMvHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "MVAS|Health")
	FMvHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "MVAS|Health")
	FMvHealthDeathEvent OnDeathStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "MVAS|Health")
	FMvHealthDeathEvent OnDeathFinished;
	
	UMvHealthComponent(const FObjectInitializer& ObjInitializer);

	UFUNCTION(BlueprintPure, Category = "MVAS|Health")
	static UMvHealthComponent* FindHealthComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UMvHealthComponent>() : nullptr;
	}

	void InitWithAbilitySystem(UMvAbilitySystemComponent* InASC);
	void UninitializeFromAbilitySystem();

	virtual void StartDeath();
	virtual void FinishDeath();

	UFUNCTION(BlueprintCallable, Category = "MVAS|Health")
	float GetHealth() const { return HealthSet ? HealthSet->GetHealth() : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "MVAS|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Health")
	float GetMaxHealth() const { return HealthSet ? HealthSet->GetMaxHealth() : 0.0f; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "MVAS|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsAlive() const { return DeathState == EMvDeathState::Alive; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "MVAS|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return DeathState > EMvDeathState::Alive; }
	
protected:
	UPROPERTY()
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<const UMvHealthSet> HealthSet;

	EMvDeathState DeathState;

	virtual void OnUnregister() override;

	void HandleHealthChanged(
		AActor* Instigator,
		AActor* Causer,
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);
	
	void HandleMaxHealthChanged(
		AActor* Instigator,
		AActor* Causer, 
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);

	void HandleOutOfHealth(
		AActor* Instigator,
		AActor* Causer,
		const FGameplayEffectSpec* EffectSpec,
		float Magnitude,
		float OldValue,
		float NewValue);
};
