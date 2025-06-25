// Copyright 2025 Reflection Tree. All Rights Reserved.

#include "AbilitySystem/AbilityTasks/GoToAbilityTask.h"
#include "RTreeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RTreeTutLogging.h"

UGoToAbilityTask* UGoToAbilityTask::CreateGoToTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector TargetDestination)
{
	UGoToAbilityTask* GoToTask = NewAbilityTask<UGoToAbilityTask>(OwningAbility, TaskInstanceName);
	GoToTask->Destination = TargetDestination;
	return GoToTask;
}

void UGoToAbilityTask::Activate()
{
	Super::Activate();
	AActor* AvatarActor = GetAvatarActor();
	if (!AvatarActor)
	{
		EndTask();
		OnFailed.Broadcast();
		return;
	}
	RTREETUT_LOGFMT(Log, "Activating GoTo ability task");
	ARTreeCharacter* Character = Cast<ARTreeCharacter>(AvatarActor);
	if (Character)
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		{
			RTREETUT_LOGFMT(Log, "Acquired movement component");
			MovementComponent->SetMovementMode(EMovementMode::MOVE_NavWalking);
			MovementComponent->MaxWalkSpeed = 600.f;
		}
		else
		{
			RTREETUT_LOGFMT(Log, "Failed to acquire movement component");
		}
		CachedAIController = Cast<AAIController>(Character->GetController());
	}
	if (!CachedAIController)
	{
		RTREETUT_LOGFMT(Error, "No CachedAIController");
		EndTask();
		OnFailed.Broadcast();
		return;
	}

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalLocation(Destination);
	MoveReq.SetAcceptanceRadius(5.0f);
	MoveReq.SetNavigationFilter(nullptr);
	MoveReq.SetAllowPartialPath(true);
	RTREETUT_LOGFMT(Log, "Initiating Move.");

	// initiate the move
	MoveRequestID = CachedAIController->MoveTo(MoveReq);
	RTREETUT_LOGFMT(Log, "GoToAbilityTask activated with ID: {1}", MoveRequestID.GetID());

	if (CachedAIController->GetPathFollowingComponent())
	{
		
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
			this, &UGoToAbilityTask::OnMoveCompleted
		);
		RTREETUT_LOGFMT(Log, "PathFollowingComponent->OnRequestFinished allocated size = `{1}`", CachedAIController->GetPathFollowingComponent()->OnRequestFinished.GetAllocatedSize());
	}
	else
	{
		RTREETUT_LOGFMT(Error, "CachedAIController->GetPathFollowingComponent() returned nullptr.");
		EndTask();
		OnFailed.Broadcast();
	}
}

void UGoToAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	// If we need to abort movement manually
	if (CachedAIController && MoveRequestID.IsValid())
	{
		CachedAIController->StopMovement();
		// Make sure we stop listening for any leftover move results
		if (CachedAIController->GetPathFollowingComponent())
		{
			CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UGoToAbilityTask::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	RTREETUT_LOGFMT(Log, "OnMoveCompleted called Result is : {1}", Result.ToString());

	// Unbind since we got our callback
	if (CachedAIController && CachedAIController->GetPathFollowingComponent())
	{
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}
	
	RTREETUT_LOGFMT(Log, "Result.Code is {1}", Result.Code);
	
	EndTask();
	
	// Check the result
	switch (Result.Code)
	{
	case EPathFollowingResult::Success:
		RTREETUT_LOGFMT(Log, "GoToAbilityTask move successful.");
		OnCompleted.Broadcast();
		break;
	case EPathFollowingResult::Aborted:
		OnInterrupted.Broadcast();
		break;
	case EPathFollowingResult::Blocked:
	case EPathFollowingResult::OffPath:
	case EPathFollowingResult::Invalid:
	default:
		OnFailed.Broadcast();
		break;
	}
}