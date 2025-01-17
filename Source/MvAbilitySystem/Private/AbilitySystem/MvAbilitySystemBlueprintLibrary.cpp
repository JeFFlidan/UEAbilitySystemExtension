// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemBlueprintLibrary.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemBlueprintLibrary)

const FGameplayEffectSpec& UMvAbilitySystemBlueprintLibrary::GetOwnerSpec(
	const FGameplayEffectCustomExecutionParameters& InExecutionParams)
{
	return InExecutionParams.GetOwningSpec();
}

FGameplayTagContainer UMvAbilitySystemBlueprintLibrary::GetSourceTags(
	const FGameplayEffectCustomExecutionParameters& InExecutionParams)
{
	if (const FGameplayTagContainer* TagContainer = InExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags())
	{
		return *TagContainer;
	}
	
	return FGameplayTagContainer();
}

FGameplayTagContainer UMvAbilitySystemBlueprintLibrary::GetTargetTags(
	const FGameplayEffectCustomExecutionParameters& InExecutionParams)
{
	if (const FGameplayTagContainer* TagContainer = InExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags())
	{
		return *TagContainer;
	}
	
	return FGameplayTagContainer();
}

bool UMvAbilitySystemBlueprintLibrary::AttemptCalculateCapturedAttributeMagnitude(
	const FGameplayEffectCustomExecutionParameters& InExecutionParams,
	const TArray<FGameplayEffectAttributeCaptureDefinition>& InRelevantAttributesToCapture,
	FGameplayAttribute InAttribute,
	float& OutMagnitude
)
{
	if (const FGameplayEffectAttributeCaptureDefinition* CaptureDefinition = FindGameplayEffectAttributeCaptureDefinition(
		InRelevantAttributesToCapture, InAttribute))
	{
		FAggregatorEvaluateParameters EvaluateParams;
		FillAggregatorEvaluateParameters(InExecutionParams, EvaluateParams);

		return InExecutionParams.AttemptCalculateCapturedAttributeMagnitude(*CaptureDefinition, EvaluateParams, OutMagnitude);
	}
	
	return false;
}

FGameplayEffectCustomExecutionOutput& UMvAbilitySystemBlueprintLibrary::AddOutputModifier(
	FGameplayEffectCustomExecutionOutput& ExecutionOutput,
	FGameplayAttribute Attribute,
	TEnumAsByte<EGameplayModOp::Type> ModOp,
	float Magnitude)
{
	ExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attribute, ModOp, Magnitude));
	return ExecutionOutput;
}


void UMvAbilitySystemBlueprintLibrary::FillAggregatorEvaluateParameters(
	const FGameplayEffectCustomExecutionParameters& InExecutionParams,
	FAggregatorEvaluateParameters& OutParams
)
{
	const FGameplayEffectSpec& Spec = InExecutionParams.GetOwningSpec();

	OutParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	OutParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
}

const FGameplayEffectAttributeCaptureDefinition* UMvAbilitySystemBlueprintLibrary::FindGameplayEffectAttributeCaptureDefinition(
	const TArray<FGameplayEffectAttributeCaptureDefinition>& RelevantAttributesToCapture,
	const FGameplayAttribute& Attribute
)
{
	for (const auto& CaptureDefinition : RelevantAttributesToCapture)
	{
		if (CaptureDefinition.AttributeToCapture == Attribute)
		{
			return &CaptureDefinition;
		}
	}

	UE_LOG(LogMvAbilitySystem, Error, TEXT("Failed to find attribute capture definition for %s"), *Attribute.AttributeName);
	return nullptr;
}
