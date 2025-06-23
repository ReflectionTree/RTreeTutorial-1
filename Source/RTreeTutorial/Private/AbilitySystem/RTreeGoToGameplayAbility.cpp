// Copyright Reflection Tree 2025, Inc. All Rights Reserved.


#include "AbilitySystem/RTreeGoToGameplayAbility.h"
#include"AbilitySystem/AbilityTasks/GoToAbilityTask.h"
#include "RTreeTutorialAIController.h"
#include "RTreeTutLogging.h"

void URTreeGoToGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get the avatar actor from the ActorInfo
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Cast the avatar to APawn since we need it to get the controller
	APawn* Pawn = Cast<APawn>(AvatarActor);
	if (!Pawn)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Get and cast to the AI Controller
	ARTreeTutorialAIController* AIController = Cast<ARTreeTutorialAIController>(Pawn->GetController());
	if (!AIController)
	{
		// The pawn either has no controller or it's not an AI Controller
		RTREETUT_LOGFMT(Error, "AIController is null when attempting to execute GoTo ability.");
		OnFailed();
		return;
	}
	// Dequeue the function call
	TSharedPtr<FJsonObject> ValidatedCallJsonObj = AIController->GetCurrentPlanStepJson();
	if (ValidatedCallJsonObj == nullptr)
	{
		RTREETUT_LOGFMT(Error, "FJsonObject is null when attempting to execute GoTo ability.");
		OnFailed();
		return;
	}
	TSharedPtr<FJsonValue> DestinationName = ValidatedCallJsonObj->TryGetField(TEXT("Destination"));
	if (!DestinationName)
	{
		RTREETUT_LOGFMT(Error, "Required Destination name not found in supplied validated call.");
		OnFailed();
		return;
	}

	if (const FVector* DestinationVecPtr = AIController->DiscoveredLocations.Find(DestinationName->AsString()))
	{
		FVector Destination = *DestinationVecPtr;
		RTREETUT_LOGFMT(Log, "Preparing to move to the destination {1}.", Destination.ToCompactString());
		UGoToAbilityTask* Task = UGoToAbilityTask::CreateGoToTask(this, "MoveTo", Destination);
		
		Task->OnCompleted.AddDynamic(this, &URTreeGoToGameplayAbility::OnCompleted);
		Task->OnFailed.AddDynamic(this, &URTreeGoToGameplayAbility::OnFailed);
		Task->OnInterrupted.AddDynamic(this, &URTreeGoToGameplayAbility::OnCancelled);
		
		Task->ReadyForActivation();
	}
	else
	{
		RTREETUT_LOGFMT(Error, "Called Destination not in Actor's DiscoveredLocations TMap.");
		OnFailed();
		return;
	}
}

TSharedRef<FJsonObject> URTreeGoToGameplayAbility::GetValidationSchema(AActor* Actor)
{
	RTREETUT_LOGFMT(Log, "Getting validation schema...");
	TSharedRef<FJsonObject> Schema = MakeShared<FJsonObject>();
	ARTreeTutorialAIController* OwningController = Cast<ARTreeTutorialAIController>(Actor);
	if (OwningController == nullptr)
	{
		RTREETUT_LOGFMT(Fatal, "Cast to RTreeTutorialAIController failed!");
		return Schema;
	}
	TArray<FString> AvailableDestinations;
	RTREETUT_LOGFMT(Log, "Number of items in DiscoveredLocations: {1}", OwningController->DiscoveredLocations.Num());
	OwningController->DiscoveredLocations.GenerateKeyArray(AvailableDestinations);
	if (AvailableDestinations.Num() == 0)
	{
		RTREETUT_LOGFMT(Warning, "No available destinations.");
		return Schema;
	}
	Schema->SetStringField("$schema", "http://json-schema.org/draft-07/schema#");
	Schema->SetStringField("title", "MoveTo");
	Schema->SetStringField("description", "Schema with valid locations for move to action.");
	Schema->SetStringField("type", "object");

	TSharedPtr<FJsonObject> PropertiesObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> DestinationObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> TitleObj = MakeShared<FJsonObject>();

	DestinationObj->SetStringField("description", "Available destinations to move to.");
	TitleObj->SetStringField("description", "MoveTo is the name of this function.");
	
	TArray<TSharedPtr<FJsonValue>> EnumValues;
	for (auto& dest : AvailableDestinations)
	{
		EnumValues.Add(MakeShared<FJsonValueString>(dest));
	}
	DestinationObj->SetArrayField("enum", EnumValues);
	TitleObj->SetStringField("title", "MoveTo");

	PropertiesObj->SetObjectField("destination", DestinationObj);
	PropertiesObj->SetObjectField("title", TitleObj);
	Schema->SetObjectField("properties", PropertiesObj);

	TArray<TSharedPtr<FJsonValue>> RequiredFields;
	RequiredFields.Add(MakeShared<FJsonValueString>("title"));
	RequiredFields.Add(MakeShared<FJsonValueString>("destination"));
	Schema->SetArrayField("required", RequiredFields);

	return Schema;
}

void URTreeGoToGameplayAbility::OnCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URTreeGoToGameplayAbility::OnFailed()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URTreeGoToGameplayAbility::OnCompleted()
{
	RTREETUT_LOGFMT(Log, "Completed GoTo ability.");
	// Get the avatar actor from the ActorInfo
	AActor* AvatarActor = GetOwningActorFromActorInfo();
	if (!AvatarActor)
	{
		RTREETUT_LOGFMT(Error, "AvatarActor is null.");
		return;
	}

	// Cast the avatar to APawn since we need it to get the controller
	APawn* Pawn = Cast<APawn>(AvatarActor);
	if (!Pawn)
	{
		RTREETUT_LOGFMT(Error, "Pawn is null.");
		return;
	}

	// Get and cast to the AI Controller
	ARTreeTutorialAIController* AIController = Cast<ARTreeTutorialAIController>(Pawn->GetController());
	if (!AIController)
	{
		// The pawn either has no controller or it's not an AI Controller
		RTREETUT_LOGFMT(Error, "AIController is null.");
		return;
	}

	AIController->HandleOnPlanStepComplete();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
