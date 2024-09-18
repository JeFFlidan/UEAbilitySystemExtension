// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "ModularPlayerController.h"
#include "MvPlayerController.generated.h"

class UObject;
class UMvAbilitySystemComponent;
class AMvPlayerState;

/**
 * 
 */
UCLASS()
class MVABILITYSYSTEM_API AMvPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
public:
	AMvPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UFUNCTION(BlueprintCallable, Category = "MVAS|PlayerController")
	AMvPlayerState* GetMvPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|PlayerController")
	UMvAbilitySystemComponent* GetMvAbilitySystemComponent() const;
};
