// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameModes/MvGameplayConfig.h"
#include "GameFeatureAction.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameplayConfig)

#define LOCTEXT_NAMESPACE "MvGameModes"

#if WITH_EDITOR
EDataValidationResult UMvGameplayConfig::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	if (!PawnData)
	{
		Result = EDataValidationResult::Invalid;
		Context.AddError(LOCTEXT("PawnDataIsNull", "Pawn Data field is empty."));
	}

	int32 EntryIndex = 0;
	for (const FString& FeatureName : GameFeatureToEnable)
	{
		if (FeatureName.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("FeatureNameIsEmpty", "Empty entry at index {0} in GameFeatureToEnable"), EntryIndex));
		}
		++EntryIndex;
	}

	EntryIndex = 0;
	for (const UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Result = CombineDataValidationResults(Result, Action->IsDataValid(Context));
		}
		else
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("ActionEntryIsNull", "Null entry at index {0} in Actions"), EntryIndex));
		}
		++EntryIndex;
	}

	if (!GetClass()->IsNative())
	{
		const UClass* ParentClass = GetClass()->GetSuperClass();

		const UClass* FirstNativeParent = ParentClass;
		while (FirstNativeParent && !FirstNativeParent->IsNative())
		{
			FirstNativeParent = FirstNativeParent->GetSuperClass();
		}

		if (FirstNativeParent != ParentClass)
		{
			Context.AddError(FText::Format(LOCTEXT("GameplayConfigInheritanceIsUnsupported", "Blueprint subclass of Blueprint Gameplay Config is unsupported. Parent class was {0} instead of {1}."),
				FText::AsCultureInvariant(GetPathNameSafe(FirstNativeParent)),
				FText::AsCultureInvariant(GetPathNameSafe(ParentClass))
			));
			Result = EDataValidationResult::Invalid;
		}
	}
	
	return Result;
}
#endif

#if WITH_EDITORONLY_DATA
void UMvGameplayConfig::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE
