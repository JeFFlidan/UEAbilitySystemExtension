// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Player/MvPlayerState.h"
#include "Character/MvPawnData.h"
#include "GameModes/MvGameMode.h"
#include "GameModes/MvGameplayConfigManagerComponent.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"
#include "AbilitySystem/MvAbilitySet.h"
#include "MvLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvPlayerState)

AMvPlayerState::AMvPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UMvAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AMvPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMvPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		UMvGameplayConfigManagerComponent* ConfigManager = GameState->FindComponentByClass<UMvGameplayConfigManagerComponent>();
		check(ConfigManager);

		ConfigManager->CallOrRegister_OnGameplayConfigLoaded(FOnMvGameplayConfigLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameplayConfigLoaded));
	}
}

UAbilitySystemComponent* AMvPlayerState::GetAbilitySystemComponent() const
{
	return GetMvAbilitySystemComponent();
}

void AMvPlayerState::SetPawnData(const UMvPawnData* InPawnData)
{
	if (PawnData)
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."),
			*GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}
	
	PawnData = InPawnData;
}

void AMvPlayerState::CallOrRegister_OnPawnDataLoaded(FOnPawnDataLoaded::FDelegate&& Delegate)
{
	if (PawnData)
	{
		Delegate.Execute(PawnData);
	}
	else
	{
		OnPawnDataLoaded.Add(MoveTemp(Delegate));
	}
}

void AMvPlayerState::OnGameplayConfigLoaded(const UMvGameplayConfig* GameplayConfig)
{
	if (AMvGameMode* MvGameMode = GetWorld()->GetAuthGameMode<AMvGameMode>())
	{
		if (const UMvPawnData* NewPawnData = MvGameMode->GetPawnData(this))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogMvAbilitySystem, Error, TEXT("AMvPlayerState::OnGameplayConfigLoaded(): Unable to find Pawn Data to init player state %s"),
				*GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogMvAbilitySystem, Error, TEXT("AMvPlayerState::OnGameplayConfigLoaded(): Failed to get Game Mode."))
	}
}
