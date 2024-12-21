// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "MvNPCStateComponent.generated.h"

class UMvAbilitySystemComponent;
class UMvPawnData;

UCLASS(ClassGroup=(MvAbilitySystem), meta=(BlueprintSpawnableComponent))
class MVABILITYSYSTEM_API UMvNPCStateComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	UMvNPCStateComponent();

	UFUNCTION(BlueprintPure, Category = "MVAS|NPC State")
	static UMvNPCStateComponent* FindNPCStateComponent(AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UMvNPCStateComponent>() : nullptr);
	}

	const UMvPawnData* GetPawnData() const { return PawnData; }
	UMvAbilitySystemComponent* GetMvAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "MVAS|NPC State")
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MVAS|NPC State")
	TObjectPtr<UMvPawnData> PawnData;
};
