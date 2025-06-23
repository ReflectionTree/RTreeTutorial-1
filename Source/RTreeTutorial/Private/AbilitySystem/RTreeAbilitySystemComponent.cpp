// Copyright RTree, 2025


#include "AbilitySystem/RTreeAbilitySystemComponent.h"

void URTreeAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		// to grant an ability, must create an ability spec first
		const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// can either pass as const
		GiveAbility(AbilitySpec);
		// 
		// or if we want to activate ability, we need to pass non const (so delete const above)
		//FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		//GiveAbilityAndActivateOnce(AbilitySpec);
	}
}