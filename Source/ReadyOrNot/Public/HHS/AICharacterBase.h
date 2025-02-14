#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

UCLASS()
class READYORNOT_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacterBase();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);

protected:
	virtual void BeginPlay() override;

private:
	class UWidgetComponent* WidgetComponent;
	float const MaxHealth{100.0f};
	float Health;

};
