// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvAbilityInputBindingComponent.h"
#include "Character/MvPawnExtensionComponent.h"
#include "Character/MvPawnData.h"
#include "Input/MvInputConfig.h"
#include "Input/MvInputComponent.h"
#include "InputMappingContext.h"
#include "MvLogChannels.h"

#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvAbilityInputBindingComponent)

UMvAbilityInputBindingComponent::UMvAbilityInputBindingComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMvAbilityInputBindingComponent::AddAdditionalInputConfig(MvInputConfig* InputConfig)
{
}

void UMvAbilityInputBindingComponent::RemoveAdditionalInputConfig(MvInputConfig* InputConfig)
{
}

bool UMvAbilityInputBindingComponent::IsReadyToBindInputs() const
{
	return false;
}

bool UMvAbilityInputBindingComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return false;
}

void UMvAbilityInputBindingComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
}

void UMvAbilityInputBindingComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
}

void UMvAbilityInputBindingComponent::CheckDefaultInitialization()
{
}

void UMvAbilityInputBindingComponent::OnRegister()
{
}

void UMvAbilityInputBindingComponent::BeginPlay()
{
}

void UMvAbilityInputBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void UMvAbilityInputBindingComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		UE_LOG(LogMvAbilitySystem, Warning, TEXT("Failed to get Pawn in UMvAbilityInputBindingComponent::InitializePlayerInput."))
		return;
	}

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	InputSubsystem->ClearAllMappings();

	if (const UMvPawnExtensionComponent* PawnExtComp = UMvPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UMvPawnData* PawnData = PawnExtComp->GetPawnData())
		{
			if (const UMvInputConfig* InputConfig = PawnData->InputConfig)
			{
				UMvInputComponent* MvIC = Cast<UMvInputComponent>(PlayerInputComponent);

				for (const FInputMappingContextInfo& MappingInfo : DefaultInputMappings)
				{
					if (UInputMappingContext* InputMapping = MappingInfo.InputMapping.Get())
					{
						if (MappingInfo.bRegisterWithSettings)
						{
							// TODO
						}

						InputSubsystem->AddMappingContext(InputMapping, MappingInfo.Priority);
					}
				}

				if (ensureMsgf(MvIC, TEXT("Unexpected Input Component class! Change the input component to UMvInputComponent or a subclass of it.")))
				{
					InputComponent = MvIC;
					MvIC->BindConstantAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);

					// TODO Basic abilities bindings
				}
			}
		}
	}
}

void UMvAbilityInputBindingComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void UMvAbilityInputBindingComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
}
