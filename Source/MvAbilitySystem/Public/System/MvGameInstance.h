// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "MvGameInstance.generated.h"

class UObject;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API UMvGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void BeginDestroy() override;
};
