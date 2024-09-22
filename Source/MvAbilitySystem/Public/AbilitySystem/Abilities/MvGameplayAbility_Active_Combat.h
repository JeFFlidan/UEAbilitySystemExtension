// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/MvGameplayAbility_Active.h"
#include "MvGameplayAbility_Active_Combat.generated.h"

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvGameplayAbility_Active_Combat : public UMvGameplayAbility_Active
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void OnEventReceived(FGameplayTag EventTag, const FGameplayEventData& EventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Combat")
	TArray<FMvAbilityMontageInfo> Montages;

	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Combat")
	float PauseHitMontage{0.05f};

	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;

	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentAnimMontage;

	void ResetMontage() const;
};
