// Copyright 2025 Reflection Tree. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Navigation/PathFollowingComponent.h"
#include "GoToAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGoToAbilityTaskDelegate);

/**
 * UGoToAbilityTask is a gameplay ability task responsible for handling
 * character movement to a specified target destination. This task utilizes
 * pathfinding capabilities, and provides various callbacks for success, failure,
 * or interruptions.
 */
UCLASS()
class RTREETUTORIAL_API UGoToAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	
	static UGoToAbilityTask* CreateGoToTask(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		FVector TargetDestination);

	virtual void Activate() override;

private:
	
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	FVector Destination;
	
	FAIRequestID MoveRequestID;
	
	UPROPERTY()
	AAIController* CachedAIController;

public:
	// Called when the task finishes successfully (character reached target)
	FGoToAbilityTaskDelegate OnCompleted;

	// Called when movement fails (unable to find path or can’t move)
	FGoToAbilityTaskDelegate OnFailed;

	// Called when the task is interrupted (ability canceled or otherwise)
	FGoToAbilityTaskDelegate OnInterrupted;
};
