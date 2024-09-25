// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "System/MvGameInstance.h"
#include "AbilitySystem/MvAbilitySystemGlobals.h"
#include "AbilitySystem/MvAbilitySystemSettings.h"
#include "MvGameplayTags.h"

#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameInstance)

void UMvGameInstance::Init()
{
	Super::Init();
	
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_DataAvailable, false, MvGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_DataInitialized, false, MvGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_GameplayReady, false, MvGameplayTags::InitState_DataInitialized);
	}

	const UMvAbilitySystemSettings* Settings = GetDefault<UMvAbilitySystemSettings>();
	UMvAbilitySystemGlobals& Globals = UMvAbilitySystemGlobals::Get();
	
	Globals.AddAttributeDefaultTables(Globals.AttributeDefaultsOwnerName, Settings->GetSoftObjectPaths());
}

void UMvGameInstance::BeginDestroy()
{
	const UMvAbilitySystemSettings* Settings = GetDefault<UMvAbilitySystemSettings>();
	UMvAbilitySystemGlobals& Globals = UMvAbilitySystemGlobals::Get();

	Globals.RemoveAttributeDefaultTables(Globals.AttributeDefaultsOwnerName, Settings->GetSoftObjectPaths());
	
	Super::BeginDestroy();
}
