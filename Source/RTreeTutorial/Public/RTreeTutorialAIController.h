// Copyright 2025 Reflection Tree. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RTreeAIController.h"
#include "RTreeTutorialAIController.generated.h"

/**
 * A tutorial implementation of an RTree AI Controller.
 * Responsible for processing environmental observations and tracking key locations.
 */
UCLASS()
class RTREETUTORIAL_API ARTreeTutorialAIController : public ARTreeAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();
	virtual FString ProcessObservations(TArray<RTreeObservation>& Observations) override;

	UPROPERTY()
	TMap<FString, FVector> DiscoveredLocations;
};
