// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "GameplayEffectComponent.h"
#include "ScalableFloat.h"

#include "MvChanceToApplyWithGuaranteeGameplayEffectComponent.generated.h"

UCLASS(DisplayName = "Chance To Apply This Effect With Guarantee")
class MVABILITYSYSTEM_API UMvChanceToApplyWithGuaranteeGameplayEffectComponent : public UGameplayEffectComponent
{
    GENERATED_BODY()

public:
    virtual void PostLoad() override;
    
    virtual bool CanGameplayEffectApply(
        const FActiveGameplayEffectsContainer& ActiveGEContainer,
        const FGameplayEffectSpec& GESpec
    ) const override;

private:
    /** Probability that this gameplay effect will be applied to the target actor (0.0 for never, 1.0 for always) */
    UPROPERTY(EditDefaultsOnly, Category = Application, meta = (GameplayAttribute = "True"))
    FScalableFloat BaseChanceToApplyToTarget;

    /**
     * A bonus chance that will be added to the BaseChanceToApplyToTarget with each activation of until guaranteed application.
     * For example, bonus chance is 0.05 and there is the fourth activation, so 4 * 0.05 will be added to BaseChanceToApplyToTarget.
     * Will be used only if ActivationCountForGuaranteedApplication != 0
     * Range from 0.0 to 1.0.
     */
    UPROPERTY(EditDefaultsOnly, Category = Application, meta = (GameplayAttribute = "True"))
    FScalableFloat BonusChanceToApplyToTarget;

    /** Count of activations of a gameplay effect that an actor must make to get a guaranteed application.
     * Must be an integer. All fractional parts will be discarded. If 0, no bonus chances will be applied.
     */
    UPROPERTY(EditDefaultsOnly, Category = Application, meta = (GameplayAttribute = "True"))
    FScalableFloat ActivationCountForGuaranteedApplication = 0;
};
