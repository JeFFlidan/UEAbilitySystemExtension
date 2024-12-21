// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvHeroComponent.h"
#include "Character/MvPawnExtensionComponent.h"
#include "Character/MvPawnData.h"
#include "Character/MvNPCStateComponent.h"
#include "Input/MvInputConfig.h"
#include "Input/MvInputComponent.h"
#include "Player/MvPlayerState.h"
#include "MvLogChannels.h"
#include "MvGameplayTags.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvHeroComponent)

UMvHeroComponent::UMvHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

void UMvHeroComponent::AddAdditionalInputConfig(const UMvInputConfig* InputConfig)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (UMvPawnExtensionComponent::FindPawnExtensionComponent(Pawn) && GetInputSubsystem())
	{
		UMvInputComponent* MvIC = Pawn->FindComponentByClass<UMvInputComponent>();
		
		if (ensureMsgf(MvIC, TEXT("Unexpected Input Component class! Change the input component to UMvInputComponent or a subclass of it.")))
		{
			MvIC->BindConstantAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);
		}
	}
}

void UMvHeroComponent::RemoveAdditionalInputConfig(const UMvInputConfig* InputConfig)
{
	// TODO
}

bool UMvHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == MvGameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == MvGameplayTags::InitState_Spawned && DesiredState == MvGameplayTags::InitState_DataAvailable)
	{
		if (UMvNPCStateComponent::FindNPCStateComponent(GetOwner()))
		{
			return true;
		}
		
		if (!GetPlayerState<AMvPlayerState>())
		{
			return false;
		}
		
		AController* Controller = GetController<AController>();
		
		bool bHasControllerPossessedPlayerState = Controller &&
			Controller->PlayerState && Controller->PlayerState->GetOwner() == Controller;

		if (!bHasControllerPossessedPlayerState)
		{
			return false;
		}

		if (Pawn->IsLocallyControlled() && !Pawn->IsBotControlled())
		{
			APlayerController* PlayerController = GetController<APlayerController>();

			if (!Pawn->InputComponent || !PlayerController || !PlayerController->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	if (CurrentState == MvGameplayTags::InitState_DataAvailable && DesiredState == MvGameplayTags::InitState_DataInitialized)
	{
		return Manager->HasFeatureReachedInitState(Pawn, UMvPawnExtensionComponent::NAME_ActorFeatureName, MvGameplayTags::InitState_DataInitialized);
	}
	if (CurrentState == MvGameplayTags::InitState_DataInitialized && DesiredState == MvGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
}

void UMvHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == MvGameplayTags::InitState_DataAvailable && DesiredState == MvGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AMvPlayerState* PlayerState = GetPlayerState<AMvPlayerState>();

		UMvNPCStateComponent* NpcState = UMvNPCStateComponent::FindNPCStateComponent(Pawn);
		if (!NpcState && !ensure(Pawn && PlayerState))
		{
			return;
		}

		if (UMvPawnExtensionComponent* PawnExtComponent = UMvPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (NpcState)
			{
				PawnExtComponent->InitializeAbilitySystem(NpcState->GetMvAbilitySystemComponent(), GetOwner());
			}
			else
			{
				PawnExtComponent->InitializeAbilitySystem(PlayerState->GetMvAbilitySystemComponent(), PlayerState);
			}
		}

		if (GetController<APlayerController>())
		{
			if (Pawn->InputComponent)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UMvHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UMvPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == MvGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void UMvHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		MvGameplayTags::InitState_Spawned,
		MvGameplayTags::InitState_DataAvailable,
		MvGameplayTags::InitState_DataInitialized,
		MvGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

void UMvHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("UMvHeroComponent::OnRegister(): This component has been added to a blueprint whose base class is not a Pawn."))
	}
	else
	{
		RegisterInitStateFeature();
	}
}

void UMvHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UMvPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(MvGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UMvHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
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

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetInputSubsystem();

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

	if (!bReadyToBindInputs)
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(GetController<APlayerController>(), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);

	UE_LOG(LogMvAbilitySystem, Log, TEXT("Initialized player input."))
}

void UMvHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	if (const UMvPawnExtensionComponent* PawnExtComponent = UMvPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UMvAbilitySystemComponent* AbilitySystemComponent = PawnExtComponent->GetMvAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagPressed(InputTag);
		}
	}
}

void UMvHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	if (const UMvPawnExtensionComponent* PawnExtComponent = UMvPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UMvAbilitySystemComponent* AbilitySystemComponent = PawnExtComponent->GetMvAbilitySystemComponent())
		{
			AbilitySystemComponent->AbilityInputTagReleased(InputTag);
		}
	}
}

UEnhancedInputLocalPlayerSubsystem* UMvHeroComponent::GetInputSubsystem()
{
	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	return InputSubsystem;
}
