// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "MvGameplayAbility.generated.h"

class UObject;
class UMvAbilitySystemComponent;
class AController;
class AMvCharacter;
class UMvHeroComponent;

/**
 * 
 */
UCLASS(ClassGroup = AbilitySystem, meta = (Category = "Mv Ability System", DisplayName = "Mv Ability"))
class MVABILITYSYSTEM_API UMvGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMvGameplayAbility(const FObjectInitializer& ObjInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability")
	UMvAbilitySystemComponent* GetMvAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability")
	AMvCharacter* GetMvCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Ability")
	UMvHeroComponent* GetMvAbilityInputBindingComponent() const;

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
	{
		NativeOnAbilityFailedToActivate(FailedReason);
		BP_OnAbilityFailedToActivate(FailedReason);
	}

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void OnPawnAvatarSet();
	
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "MVAS|Ability", DisplayName = "OnAbilityFailedToActivate")
	void BP_OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "MVAS|Ability", DisplayName = "OnAbilityAdded")
	void BP_OnAbilityAdded();

	UFUNCTION(BlueprintImplementableEvent, Category = "MVAS|Ability", DisplayName = "OnAbilityRemoved")
	void BP_OnAbilityRemoved();

	UFUNCTION(BlueprintImplementableEvent, Category = "MVAS|Ability", DisplayName = "OnPawnAvatarSet")
	void BP_OnPawnAvatarSet();
};
