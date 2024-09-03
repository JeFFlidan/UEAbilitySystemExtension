// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "WorldActionBase_GameFeatureAction.h"
#include "UObject/SoftObjectPtr.h"

#include "AddInputBinding_GameFeatureAction.generated.h"

UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Binds"))
class UAddInputBinding_GameFeatureAction final : public UWorldActionBase_GameFeatureAction
{
    GENERATED_BODY()
};