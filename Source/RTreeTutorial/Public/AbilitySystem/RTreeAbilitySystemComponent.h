// Copyright 2025 Reflection Tree. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RTreeAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTREETUTORIAL_API URTreeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
};
