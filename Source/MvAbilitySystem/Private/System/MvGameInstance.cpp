// Copyright Kyrylo Zaverukha. All Rights Reserved.

#include "System/MvGameInstance.h"
#include "MvGameplayTags.h"
#include "MvLogChannels.h"

#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MvGameInstance)

void UMvGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMvAbilitySystem, Log, TEXT("Init custom game instance"))
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_DataAvailable, false, MvGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_DataInitialized, false, MvGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(MvGameplayTags::InitState_GameplayReady, false, MvGameplayTags::InitState_DataInitialized);
	}
}
