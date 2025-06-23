// Copyright RTree, 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "RTreeCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UAttributeSet;

UCLASS()
class RTREETUTORIAL_API ARTreeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTreeCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	void AddCharacterAbilities();
private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
