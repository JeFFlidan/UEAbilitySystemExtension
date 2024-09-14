// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvHeroComponent.h"
#include "Character/MvPawnExtensionComponent.h"
#include "Character/MvPawnData.h"
#include "Input/MvInputConfig.h"
#include "Input/MvInputComponent.h"
#include "InputMappingContext.h"
#include "MvLogChannels.h"

#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvHeroComponent)

UMvHeroComponent::UMvHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMvHeroComponent::AddAdditionalInputConfig(const UMvInputConfig* InputConfig)
{
}

void UMvHeroComponent::RemoveAdditionalInputConfig(const UMvInputConfig* InputConfig)
{
}

bool UMvHeroComponent::IsReadyToBindInputs() const
{
	return false;
}

bool UMvHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return false;
}

void UMvHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
}

void UMvHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
}

void UMvHeroComponent::CheckDefaultInitialization()
{
}

void UMvHeroComponent::OnRegister()
{
}

void UMvHeroComponent::BeginPlay()
{
}

void UMvHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void UMvHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		UE_LOG(LogMvAbilitySystem, Warning, TEXT("Failed to get Pawn in UMvHeroComponent::InitializePlayerInput."))
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

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PlayerController), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UMvHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void UMvHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
}
