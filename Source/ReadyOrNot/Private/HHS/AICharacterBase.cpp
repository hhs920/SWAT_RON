#include "HHS/AICharacterBase.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"


AAICharacterBase::AAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAICharacterBase::GetHealth() const
{
	return Health;
}

float AAICharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

void AAICharacterBase::SetHealth(float const NewHealth)
{
	Health = NewHealth;
}

