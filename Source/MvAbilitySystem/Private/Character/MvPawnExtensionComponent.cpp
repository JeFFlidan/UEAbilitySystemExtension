// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "Character/MvPawnExtensionComponent.h"
#include "Character/MvPawnData.h"
#include "MvLogChannels.h"

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
}

bool UMvPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	return false;
}

void UMvPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
}

void UMvPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
}

void UMvPawnExtensionComponent::CheckDefaultInitialization()
{
}

void UMvPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
}

void UMvPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMvPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
