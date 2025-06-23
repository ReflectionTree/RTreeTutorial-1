// Copyright RTree, 2025


#include "RTreeCharacter.h"
#include "AbilitySystem/RTreeAbilitySystemComponent.h"

ARTreeCharacter::ARTreeCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<URTreeAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ARTreeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARTreeCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

}

void ARTreeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AddCharacterAbilities();
}

void ARTreeCharacter::AddCharacterAbilities()
{
	URTreeAbilitySystemComponent* RTreeASC = CastChecked<URTreeAbilitySystemComponent>(AbilitySystemComponent);
	RTreeASC->AddCharacterAbilities(StartupAbilities);
}


