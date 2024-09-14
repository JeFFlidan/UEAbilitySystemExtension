// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MvGameplayAbility.h"
#include "MvGameplayAbility_Active.generated.h"


/**
 * Defines how an ability is activated
 */
UENUM(BlueprintType)
enum class EMvAbilityActivationPolicy : uint8
{
	OnInputTriggered UMETA(DisplayName = "OnInputTriggered"),
	WhileInputActive UMETA(DisplayName = "WhileInputActive"),
	Max UMETA(Hidden)
};

/**
 * Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class EMvAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities
	Independent UMETA(DisplayName = "Independent"),

	// Ability is canceled and replaced by other dependent abilities
	Dependent_Replaceable UMETA(DisplayName = "Dependent_Replaceable"),

	// Ability blocks all other dependent abilities from activating
	Dependent_Blocking UMETA(DisplayName = "Dependent_Blocking"),

	MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvGameplayAbility_Active : public UMvGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMvGameplayAbility_Active(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EMvAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EMvAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MVAS|Ability Activation")
	EMvAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MVAS|Ability Activation")
	EMvAbilityActivationGroup ActivationGroup;

	// If true, extra information will be logged when this ability is cancelled. For tracking bugs only
	UPROPERTY(EditDefaultsOnly, Category = "MVAS|Advanced")
	bool bLogCancelation;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
