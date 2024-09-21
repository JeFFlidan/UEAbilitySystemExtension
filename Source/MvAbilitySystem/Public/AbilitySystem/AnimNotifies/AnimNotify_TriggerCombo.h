// Copyright Kyrylo Zaverukha. All Rights Reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_TriggerCombo.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Trigger Combo"))
class MVABILITYSYSTEM_API UAnimNotify_TriggerCombo : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
