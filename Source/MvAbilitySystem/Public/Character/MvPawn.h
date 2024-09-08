// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularPawn.h"
#include "MvPawn.generated.h"

UCLASS()
class MVABILITYSYSTEM_API AMvPawn : public AModularPawn
{
	GENERATED_BODY()

public:
	AMvPawn();

protected:
	virtual void BeginPlay() override;
};
