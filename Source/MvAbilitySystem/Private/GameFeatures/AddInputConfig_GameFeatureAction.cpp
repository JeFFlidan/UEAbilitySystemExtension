// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameFeatures/AddInputConfig_GameFeatureAction.h"
#include "Input/MvInputConfig.h"
#include "Character/MvHeroComponent.h"
#include "MvLogChannels.h"

#include "GameFramework/Pawn.h"
#include "Engine/LocalPlayer.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/GameInstance.h"
#include "EnhancedInputSubsystems.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(AddInputConfig_GameFeatureAction)

#define LOCTEXT_NAMESPACE "GameFeatures"

void UAddInputConfig_GameFeatureAction::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();
}

void UAddInputConfig_GameFeatureAction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = Contexts.FindOrAdd(Context);
	if (!ensure(ActiveData.PawnsAddedTo.IsEmpty()) || !ensure(ActiveData.ExtensionRequestHandles.IsEmpty()))
	{
		Reset(ActiveData);
	}
	Super::OnGameFeatureActivating(Context);
}

void UAddInputConfig_GameFeatureAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* ActiveData = Contexts.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UAddInputConfig_GameFeatureAction::OnGameFeatureUnregistering()
{
	Super::OnGameFeatureUnregistering();
}

#if WITH_EDITOR
EDataValidationResult UAddInputConfig_GameFeatureAction::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 Index = 0;

	for (const TSoftObjectPtr<UMvInputConfig>& Entry : InputConfigs)
	{
		if (Entry.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NullInputConfig", "Null input config at index {0}."), Index));
		}
		++Index;
	}
	
	return Result;
}
#endif

void UAddInputConfig_GameFeatureAction::AddToWorld(
	const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = World->GetGameInstance();
	FPerContextData& ActiveData = Contexts.FindOrAdd(ChangeContext);

	if (GameInstance && World && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = GameInstance->GetSubsystem<UGameFrameworkComponentManager>())
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddInputBindingsDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
				ComponentManager->AddExtensionHandler(APawn::StaticClass(), AddInputBindingsDelegate);

			ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
		}
	}
}

void UAddInputConfig_GameFeatureAction::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();

		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}

		ActiveData.PawnsAddedTo.Pop();
	}
}

void UAddInputConfig_GameFeatureAction::HandlePawnExtension(
	AActor* Actor,
	FName EventName,
	FGameFeatureStateChangeContext ChangeContext)
{
	APawn* Pawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = Contexts.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveInputConfig(Pawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UMvHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(Pawn, ActiveData);
	}
}

void UAddInputConfig_GameFeatureAction::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());

	if (ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			UMvHeroComponent* HeroComponent = UMvHeroComponent::FindHeroComponent(Pawn);
			if (HeroComponent && HeroComponent->IsReadyToBindInputs())
			{
				for (const TSoftObjectPtr<UMvInputConfig>& Entry : InputConfigs)
				{
					if (const UMvInputConfig* BindSet = Entry.Get())
					{
						HeroComponent->AddAdditionalInputConfig(BindSet);
					}
				}

				ActiveData.PawnsAddedTo.AddUnique(Pawn);
			}
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("Failed to find UEnhancedInputLocalPlayerSubsystem for local player. Input configs will not be applied."));
		}
	}
}

void UAddInputConfig_GameFeatureAction::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());

	if (ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			UMvHeroComponent* HeroComponent = UMvHeroComponent::FindHeroComponent(Pawn);
			if (HeroComponent && HeroComponent->IsReadyToBindInputs())
			{
				for (const TSoftObjectPtr<UMvInputConfig>& Entry : InputConfigs)
				{
					if (const UMvInputConfig* BindSet = Entry.Get())
					{
						HeroComponent->RemoveAdditionalInputConfig(BindSet);
					}
				}
			}
		}
	}

	ActiveData.PawnsAddedTo.Remove(Pawn);
}

#undef LOCTEXT_NAMESPACE
