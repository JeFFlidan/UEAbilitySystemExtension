// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "AbilitySystem/MvAbilitySet.h"
#include "AbilitySystem/Abilities/MvGameplayAbility.h"
#include "ActiveGameplayEffectHandle.h"
#include "MvGameplayAbility_Passive.generated.h"

class UObject;

/**
 * 
 */
UCLASS(meta = (DisplayName = "Mv Passive Ability"))
class MVABILITYSYSTEM_API UMvGameplayAbility_Passive : public UMvGameplayAbility
{
	GENERATED_BODY()

	friend class UMvGameplayAbilityValidator;

public:
	UMvGameplayAbility_Passive(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void DraftAdditionalGameplayEffect();
	const TArray<FMvAbilitySet_GameplayEffect>& GetDraftedGameplayEffects() const
	{
		return DraftedGameplayEffects;
	}
	
	void RemoveGrantedGameplayEffects();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Gameplay Effects")
	FMvAbilitySet_GameplayEffect MainGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Gameplay Effects")
	TArray<FMvAbilitySet_GameplayEffect> AdditionalGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Gameplay Effects", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float AdditionalGameplayEffectDropChance;

	UPROPERTY()
	TArray<FMvAbilitySet_GameplayEffect> DraftedGameplayEffects;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GrantedGameplayEffectHandles;

	bool IsTriggeredByGameplayEvent() const;
	UGameplayEffect* GetGameplayEffectCDO(const FMvAbilitySet_GameplayEffect& EffectInfo) const;

public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
