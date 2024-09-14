// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "GameFeatures/AddInputContextMapping_GameFeatureAction.h"
#include "Character/MvHeroComponent.h"
#include "MvLogChannels.h"

#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(AddInputContextMapping_GameFeatureAction)

#define LOCTEXT_NAMESPACE "MvGameFeatures"

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();
}

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	if (!ensure(ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ControllersAddedTo.IsEmpty()))
	{
		Reset();
	}

	Super::OnGameFeatureActivating(Context);
}

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureUnregistering()
{
	Super::OnGameFeatureUnregistering();
}

#if WITH_EDITOR
EDataValidationResult UAddInputContextMapping_GameFeatureAction::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 Index = 0;

	for (const FInputMappingContextInfo& Entry : InputMappings)
	{
		if (Entry.InputMapping.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NullInputMapping", "Null InputMapping at index {0}."), Index));
		}
		++Index;
	}

	return Result;
}
#endif

void UAddInputContextMapping_GameFeatureAction::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	const UWorld* World{WorldContext.World()};
	const UGameInstance* GameInstance{WorldContext.OwningGameInstance.Get()};

	if (GameInstance != nullptr && World != nullptr && World->IsGameWorld() && !InputMappings.IsEmpty())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			const UGameFrameworkComponentManager::FExtensionHandlerDelegate Delegate{
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleControllerExtension)};

			ExtensionRequestHandles.Add(ComponentManager->AddExtensionHandler(APlayerController::StaticClass(), Delegate));
		}
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("Failed to add input mapping contexts to the player controller."));
	}
}

void UAddInputContextMapping_GameFeatureAction::Reset()
{
	ExtensionRequestHandles.Empty();

	while (!ControllersAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APlayerController> Controller = ControllersAddedTo.Top();
		if (Controller.IsValid())
		{
			RemoveInputMappings(Controller.Get());
		}
		
		ControllersAddedTo.Pop();
	}
}

void UAddInputContextMapping_GameFeatureAction::HandleControllerExtension(AActor* InActor, FName EventName)
{
	APlayerController* Controller = CastChecked<APlayerController>(InActor);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveInputMappings(Controller);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UMvHeroComponent::NAME_BindInputsNow)
	{
		AddInputMappings(Controller);
	}
}

void UAddInputContextMapping_GameFeatureAction::AddInputMappings(APlayerController* PlayerController)
{
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (FInputMappingContextInfo& Entry : InputMappings)
			{
				InputSystem->AddMappingContext(Entry.InputMapping.Get(), Entry.Priority);

				// TODO: Add input mapping to EnhancedInputUserSettings
			}
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("Failed to find UEnhancedInputLocalPlayerSubsystem for local player. Input mappings will not be added. Make sure you are set to use the EnhancedInput system via config file."));
		}
	}
}

void UAddInputContextMapping_GameFeatureAction::RemoveInputMappings(APlayerController* PlayerController)
{
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (FInputMappingContextInfo& Entry : InputMappings)
			{
				InputSystem->RemoveMappingContext(Entry.InputMapping.Get());
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE