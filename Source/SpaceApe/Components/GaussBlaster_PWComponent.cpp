// Fill out your copyright notice in the Description page of Project Settings.

#include "GaussBlaster_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/ObjectPoolComponent.h"
#include "UObject/ConstructorHelpers.h"

UGaussBlaster_PWComponent::UGaussBlaster_PWComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("/Game/Particles/Blaster_Particle"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/KKIIDDZZ_00018"));


	TArray<class UProjectileComponent*> ProjectileComponentArray;

	WeaponData = FWeaponData(
		ProjectileMeshAsset.Object,
		ProjectileParticleAsset.Object,
		FireAudio.Object,
		ProjectileComponentArray,
		0.2f,
		45,
		2000
	);
}

void UGaussBlaster_PWComponent::Shoot(FVector _FireDirection) {
	const FRotator FireRotation = _FireDirection.Rotation();

	FVector GunOffset = FVector(0.f, 0.f, -30.f);

	const FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

	ASpaceApeProjectile* Projectile = Cast<ASpaceApeProjectile>(PlayerProjectilePoolRef->GetReusableReference());


	FVector SpawnLocationA = SpawnLocation + FVector(20, 20, 0);//FVector::CrossProduct(SpawnLocation, _FireDirection) ;
	FVector SpawnLocationB = SpawnLocation - FVector(20, 20, 0);

	if (Projectile != nullptr) {
		//Projectile->ToggleEnabled(true); // moved to the projectile's multicast. If needed, then a bool param could be added to the multicast method.
		Projectile->SetProjectileLocationAndDirection(SpawnLocationA, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
	ASpaceApeProjectile* ProjectileB = Cast<ASpaceApeProjectile>(PlayerProjectilePoolRef->GetReusableReference());

	if (ProjectileB != nullptr) {
		ProjectileB->SetProjectileLocationAndDirection(SpawnLocationB, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
}
