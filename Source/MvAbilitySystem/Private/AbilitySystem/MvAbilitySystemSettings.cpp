// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "AbilitySystem/MvAbilitySystemSettings.h"
#include "AbilitySystem/MvAbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilitySystemSettings)

UMvAbilitySystemSettings::UMvAbilitySystemSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	CategoryName = "Plugins";
	SectionName = "Mv Ability System";
}

TArray<FSoftObjectPath> UMvAbilitySystemSettings::GetSoftObjectPaths() const
{
	TArray<FSoftObjectPath> Paths;
	
	for (const TSoftObjectPtr<UCurveTable>& CurveTable : AttributeSetDefaultsCurveTables)
	{
		Paths.AddUnique(CurveTable.ToSoftObjectPath());
	}

	return Paths;
}
