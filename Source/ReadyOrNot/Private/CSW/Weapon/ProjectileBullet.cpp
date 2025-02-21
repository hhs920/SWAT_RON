// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/ProjectileBullet.h"

#include "Components/BoxComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AProjectileBullet::AProjectileBullet() : AProjectile()
{
	CollisionBox->SetBoxExtent(FVector(5.f, 2.5f, 2.5f));
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector Normal, const FHitResult& HitResult)
{
	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner()); // ProjectileWeapon의 SpawnParams에서 Owner 세팅이 됐음.!
	if (OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->GetController();
		if (OwnerController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController,
				this, UDamageType::StaticClass());	
		}
	}
	

	// Destroy()하므로 마지막에 호출
	Super::OnHit(HitComp, OtherActor, OtherComp, Normal, HitResult); 
}
