// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "MvAbilitySystemBlueprintLibrary.generated.h"

UCLASS()
class UMvAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "MVAS|Gameplay Effect Execution Calculation")
	static const FGameplayEffectSpec& GetOwnerSpec(const FGameplayEffectCustomExecutionParameters& ExecutionParams);
	
	UFUNCTION(BlueprintPure, Category = "MVAS|Gameplay Effect Execution Calculation")
	static FGameplayTagContainer GetSourceTags(const FGameplayEffectCustomExecutionParameters& ExecutionParams);

	UFUNCTION(BlueprintPure, Category = "MVAS|Gameplay Effect Execution Calculation")
	static FGameplayTagContainer GetTargetTags(const FGameplayEffectCustomExecutionParameters& ExecutionParams);
	
	UFUNCTION(BlueprintPure, Category = "MVAS|Gameplay Effect Execution Calculation")
	static bool AttemptCalculateCapturedAttributeMagnitude(
		const FGameplayEffectCustomExecutionParameters& InExecutionParams,
		const TArray<FGameplayEffectAttributeCaptureDefinition>& InRelevantAttributesToCapture,
		FGameplayAttribute InAttribute,
		float& OutMagnitude);

	UFUNCTION(BlueprintCallable, Category = "MVAS|Gameplay Effect Execution Calculation")
	static FGameplayEffectCustomExecutionOutput& AddOutputModifier(
		UPARAM(ref) FGameplayEffectCustomExecutionOutput& ExecutionOutput,
		FGameplayAttribute Attribute,
		TEnumAsByte<EGameplayModOp::Type> ModOp,
		float Magnitude
	);

private:
	static void FillAggregatorEvaluateParameters(
		const FGameplayEffectCustomExecutionParameters& InExecutionParams,
		FAggregatorEvaluateParameters& OutParams
	);

	static const FGameplayEffectAttributeCaptureDefinition* FindGameplayEffectAttributeCaptureDefinition(
		const TArray<FGameplayEffectAttributeCaptureDefinition>& RelevantAttributesToCapture,
		const FGameplayAttribute& Attribute
	);
};
