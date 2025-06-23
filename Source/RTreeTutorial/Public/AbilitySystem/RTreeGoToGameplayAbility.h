// Copyright RTree, 2025

#pragma once

#include "CoreMinimal.h"
#include "RTreeAbilityInterface.h"
#include "Abilities/GameplayAbility.h"
#include "RTreeGoToGameplayAbility.generated.h"

/** Class responsible for validating movement ability calls from model and executing in-game
 */
UCLASS()
class RTREETUTORIAL_API URTreeGoToGameplayAbility : public UGameplayAbility, public IRTreeAbilityInterface
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
	// IRTreeAbilityInterface start
	TSharedRef<FJsonObject> GetValidationSchema(AActor* Actor);
	// IRTreeAbilityInterface end
	
private:
	UFUNCTION()
	void OnCancelled();
	
	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnFailed();

	// UFUNCTION()
	// void EventReceived();
};
