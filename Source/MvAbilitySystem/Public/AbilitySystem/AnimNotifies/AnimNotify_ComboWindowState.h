// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_ComboWindowState.generated.h"

class UMvAbilitySystemComponent;

/**
 * 
 */
UCLASS(meta = (DisplayName = "Combo Window"))
class MVABILITYSYSTEM_API UAnimNotify_ComboWindowState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotify_ComboWindowState(const FObjectInitializer& ObjInitializer = FObjectInitializer::Get());

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyTick(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "MVAS|Combo")
	bool bEndCombo;

	UPROPERTY()
	TObjectPtr<UMvAbilitySystemComponent> AbilitySystemComponent;
};
