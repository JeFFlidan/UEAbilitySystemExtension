// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/GameplayEffectComponents/MvChanceToApplyWithGuaranteeGameplayEffectComponent.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "MvLogChannels.h"

#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvChanceToApplyWithGuaranteeGameplayEffectComponent)

#if WITH_EDITOR
#define GETCURVE_REPORTERROR_WITHPOSTLOAD(Handle)														\
	if (Handle.CurveTable) const_cast<UCurveTable*>(ToRawPtr(Handle.CurveTable))->ConditionalPostLoad(); \
GETCURVE_REPORTERROR(Handle);

#endif // WITH_EDITOR

void UMvChanceToApplyWithGuaranteeGameplayEffectComponent::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	GETCURVE_REPORTERROR_WITHPOSTLOAD(BaseChanceToApplyToTarget.Curve);
	GETCURVE_REPORTERROR_WITHPOSTLOAD(BonusChanceToApplyToTarget.Curve);
	GETCURVE_REPORTERROR_WITHPOSTLOAD(ActivationCountForGuaranteedApplication.Curve);
#endif // WITH_EDITOR
}

bool UMvChanceToApplyWithGuaranteeGameplayEffectComponent::CanGameplayEffectApply(
	const FActiveGameplayEffectsContainer& ActiveGEContainer,
	const FGameplayEffectSpec& GESpec) const
{
	const FString ContextString = GESpec.Def->GetName();

	const float CalculatedBaseChanceToApplyToTarget = BaseChanceToApplyToTarget.GetValueAtLevel(GESpec.GetLevel(), &ContextString);
	const float CalculatedBonusChanceToApplyToTarget = BonusChanceToApplyToTarget.GetValueAtLevel(GESpec.GetLevel(), &ContextString);
	const float CalculatedActivationCountForGuaranteedApplication = ActivationCountForGuaranteedApplication.GetValueAtLevel(GESpec.GetLevel(), &ContextString);

	int32 CountForGuaranteedApplication = FMath::Floor(CalculatedActivationCountForGuaranteedApplication);
	float ApplicationChance = CalculatedBaseChanceToApplyToTarget;

	if (CountForGuaranteedApplication)
	{
		UMvAbilitySystemComponent* MvASC = Cast<UMvAbilitySystemComponent>(ActiveGEContainer.Owner);
		check(MvASC);

		int32 ActivationCount = MvASC->GetGameplayEffectActivationCount(GESpec.Def.GetClass());
		int32 ActivationCountReminder = ActivationCount % CountForGuaranteedApplication;
	
		if (ActivationCountReminder == 0)
		{
			return true;
		}

		ApplicationChance = CalculatedBaseChanceToApplyToTarget + (ActivationCountReminder - 1) * CalculatedBonusChanceToApplyToTarget;
	}
	
	UE_LOG(LogMvAbilitySystem, Display, TEXT("Application Chance: %f"), ApplicationChance);

	if (ApplicationChance < 1.0f - SMALL_NUMBER && FMath::FRand() > ApplicationChance)
	{
		return false;
	}
	
	return true;
}
