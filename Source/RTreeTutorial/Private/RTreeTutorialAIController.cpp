// Copyright Reflection Tree 2025, Inc. All Rights Reserved.

#include "RTreeTutorialAIController.h"
#include "Perception/AIPerceptionComponent.h"

void ARTreeTutorialAIController::BeginPlay()
{
	Super::BeginPlay();

	UAIPerceptionComponent* PerceptionComp = GetPerceptionComponent();  // Get the actual component
	if (!PerceptionComp)
	{
		return;
	}
	if (SightConfig)
	{
		SightConfig->SightRadius = RTreeSightRadius;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(30.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		PerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());
		PerceptionComp->ConfigureSense((*SightConfig));
	}
}

FString ARTreeTutorialAIController::ProcessObservations(TArray<RTreeObservation>& Observations)
{
	FString ObservationPrompt = "[Start of Observation] You perceive the following:\n";

	for (RTreeObservation& obs : Observations)
	{
		DiscoveredLocations.Emplace(obs.GetEnumName(), obs.GetMovementLocation());
		ObservationPrompt += "Entity description: ";
		ObservationPrompt += obs.GetVisualDescription();
		ObservationPrompt += "\n";
	}
	ObservationPrompt += "[End of Observation]\n";
	return ObservationPrompt;
}
