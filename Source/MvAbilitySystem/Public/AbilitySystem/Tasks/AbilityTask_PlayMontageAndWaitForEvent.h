// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_PlayMontageAndWaitForEvent.generated.h"

class UMvGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMvPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class UAbilityTask_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjInitializer = FObjectInitializer::Get());

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override;

	// The montage completely finished playing.
	UPROPERTY(BlueprintAssignable)
	FMvPlayMontageAndWaitForEventDelegate OnCompleted;

	// The montage started blending out.
	UPROPERTY(BlueprintAssignable)
	FMvPlayMontageAndWaitForEventDelegate OnBlendOut;

	// The montage was interrupted.
	UPROPERTY(BlueprintAssignable)
	FMvPlayMontageAndWaitForEventDelegate OnInterrupted;

	// The ability task was explicitly cancelled by another ability.
	UPROPERTY(BlueprintAssignable)
	FMvPlayMontageAndWaitForEventDelegate OnCancelled;

	// One of the triggering gameplay events happened.
	UPROPERTY(BlueprintAssignable)
	FMvPlayMontageAndWaitForEventDelegate OnEventReceived;

	UFUNCTION(BlueprintCallable, Category = "MVAS|Tasks")
	static UAbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
		UMvGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer EventTags,
		float Rate = 1.0f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.0);

private:
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTagContainer EventTags;
	
	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bStopWhenAbilityEnds;
	
	bool StopPlayingMontage();

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
};
