// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/ProjectileBullet.h"

#include "Enemy.h"
#include "EnemyFSM.h"
#include "PistolEnemyFSM.h"
#include "Components/BoxComponent.h"
#include "CSW/Character/PlayerCharacter.h"
#include "HHS/PistolEnemy.h"
#include "HHS/RunEnemy.h"
#include "Kismet/GameplayStatics.h"

AProjectileBullet::AProjectileBullet() : AProjectile()
{
	CollisionBox->SetBoxExtent(FVector(5.f, 2.5f, 2.5f));
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector Normal, const FHitResult& HitResult)
{
	//APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner()); // ProjectileWeapon의 SpawnParams에서 Owner 세팅이 됐음.!
	//if (OwnerCharacter)
	//{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Damage");

	AController* OwnerController = GetOwner()->GetInstigatorController();
	if (OwnerController)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Damage");

		// 플레이어가 쏜 총알
		auto player = Cast<APlayerCharacter>(OwnerController->GetCharacter());
		if (player)
		{
			// 적이면 OnDamageProcess
			auto enemy = Cast<AEnemy>(OtherActor);
			if (enemy)
			{
				enemy->FSM->OnDamageProcess(Damage);
			}
		
			auto pistolEnemy = Cast<APistolEnemy>(OtherActor);
			if (pistolEnemy)
			{
				pistolEnemy->FSM->OnDamageProcess(Damage);
			}
			
			
		}

		// 적이 쏜 총알이면
		auto enemy = Cast<AEnemy>(OwnerController->GetCharacter());
		if (enemy)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			//enemy->FSM->OnDamageProcess(Damage);
		}
		auto pistolEnemy = Cast<APistolEnemy>(OwnerController->GetCharacter());
		if (pistolEnemy)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			//pistolEnemy->FSM->OnDamageProcess(Damage);
		}
	}
	//}


	// Destroy()하므로 마지막에 호출
	Super::OnHit(HitComp, OtherActor, OtherComp, Normal, HitResult); 
}
