// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/Weapon/ProjectileWeapon.h"

#include "CSW/Weapon/Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());

	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	
	if (InstigatorPawn && MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		FVector DirToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator RotationoTarget = DirToTarget.Rotation();
		
		if (ProjectileClass)
		{
			FActorSpawnParameters SpawnParams; // ApplyDamage에서 Owner, Instigator 정보가 사용됨!
			SpawnParams.Owner = GetOwner(); // Cast<AActor>(OwnerCharacter);
			SpawnParams.Instigator = InstigatorPawn;
			
			GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
				SocketTransform.GetLocation(), RotationoTarget, SpawnParams);
		}
	}
}
