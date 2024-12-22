// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvPawnExtensionComponent.h"
#include "Character/MvPawnData.h"
#include "Character/MvNPCStateComponent.h"
#include "Player/MvPlayerState.h"
#include "MvLogChannels.h"
#include "MvGameplayTags.h"
#include "AbilitySystem/MvAbilitySet.h"
#include "AbilitySystem/MvAbilitySystemComponent.h"

#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvPawnExtensionComponent)

UMvPawnExtensionComponent::UMvPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UMvPawnExtensionComponent::SetPawnData(const UMvPawnData* InPawnData)
{
	check(InPawnData);

	if (PawnData)
	{
		APawn* Pawn = GetPawnChecked<APawn>();
		UE_LOG(LogMvAbilitySystem, Error, TEXT("Trying to set PawnData [%s] on Pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
	}

	PawnData = InPawnData;

	CheckDefaultInitialization();
}

void UMvPawnExtensionComponent::InitializeAbilitySystem(
	UMvAbilitySystemComponent* InAbilitySystemComponent,
	AActor* InOwner)
{
	check(InAbilitySystemComponent);
	check(InOwner);

	if (AbilitySystemComponent == InAbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InAbilitySystemComponent->GetAvatarActor();

	UE_LOG(LogMvAbilitySystem, Log, TEXT("Setting up ASC %s on pawn %s owner %s, existing avatar %s"),
		*GetNameSafe(InAbilitySystemComponent), *GetNameSafe(Pawn), *GetNameSafe(InOwner), *GetNameSafe(ExistingAvatar));

	if (ExistingAvatar != nullptr && ExistingAvatar != Pawn)
	{
		if (UMvPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(InOwner, Pawn);

	for (const UMvAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	OnAbilitySystemInitialized.Broadcast();
}

void UMvPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UMvPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UMvPawnExtensionComponent::OnAbilitySystemUninitialized_RegisterAndCall(
	FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}

bool UMvPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	UE_LOG(LogMvAbilitySystem, Log, TEXT("Outside if %s %s"), *CurrentState.ToString(), *DesiredState.ToString());

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
		if (!PawnData)
		{
			UE_LOG(LogTemp, Error, TEXT("INVLAID PAWN DATA"))
			return false;
		}

		return true;
	}
	if (CurrentState == MvGameplayTags::InitState_DataAvailable && DesiredState == MvGameplayTags::InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, MvGameplayTags::InitState_DataAvailable);
	}
	if (CurrentState == MvGameplayTags::InitState_DataInitialized && DesiredState == MvGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
}

void UMvPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	
}

void UMvPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == MvGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UMvPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		MvGameplayTags::InitState_Spawned,
		MvGameplayTags::InitState_DataAvailable,
		MvGameplayTags::InitState_DataInitialized,
		MvGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

void UMvPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf(Pawn != nullptr, TEXT("MvPawnExtensionComponent on %s can only be added to Pawn actors"),
		*GetNameSafe(Pawn));

	TArray<UActorComponent*> Components;
	Pawn->GetComponents(StaticClass(), Components);
	ensureAlwaysMsgf(Components.Num() == 1, TEXT("Only one MvPawnExtensionComponent must exist on %s"),
		*GetNameSafe(GetOwner()));
	
	RegisterInitStateFeature();
}

void UMvPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	if (UMvNPCStateComponent* HeroState = UMvNPCStateComponent::FindNPCStateComponent(GetOwner()))
	{
		SetPawnData(HeroState->GetPawnData());
	}
	else
	{
		AMvPlayerState* MvPlayerState = GetPlayerState<AMvPlayerState>();
		MvPlayerState->CallOrRegister_OnPawnDataLoaded(FOnPawnDataLoaded::FDelegate::CreateUObject(this, &ThisClass::OnPawnDataLoaded));
	}

	ensure(TryToChangeInitState(MvGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UMvPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMvPawnExtensionComponent::OnPawnDataLoaded(const UMvPawnData* InPawnData)
{
	check(InPawnData);

	SetPawnData(InPawnData);
}
