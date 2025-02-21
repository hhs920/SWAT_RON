// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSW/Weapon/Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class READYORNOT_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileBullet();

protected:
	//UFUNCTION() // 부모인 Projectile의 OnHit에서 UFUNCTION()을 적용했음!
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector Normal, const FHitResult& HitResult) override;

};
