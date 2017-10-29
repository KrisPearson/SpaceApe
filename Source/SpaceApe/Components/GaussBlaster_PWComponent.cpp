// Fill out your copyright notice in the Description page of Project Settings.

#include "GaussBlaster_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/ObjectPoolComponent.h"
#include "UObject/ConstructorHelpers.h"

UGaussBlaster_PWComponent::UGaussBlaster_PWComponent() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'"));

	TArray<class UProjectileComponent*> ProjectileComponentArray;

	WeaponData = FWeaponData(
		ProjectileComponentArray, // Components to be added to the projectile actor
		ProjectileMeshAsset.Object, // Mesh used for collision events and visual appearance
		ProjectileParticleAsset.Object, // The constant visual effect particle ( for trails etc)
		nullptr,
		FireAudio.Object, // Sound effect played when fired
		nullptr,
		0.185f, // Delay between shots
		55, // Damage of projectiles
		2600 // Movement Speed of projectiles
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

		UE_LOG(LogTemp, Warning, TEXT("Component ID = %d __ Projectile ID = %d"), this->GetUniqueID(), Projectile->GetWeaponDataID() );
		CheckAndUpdateProjectile(Projectile);
		Projectile->SetProjectileLocationAndDirection(SpawnLocationA, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
	ASpaceApeProjectile* ProjectileB = Cast<ASpaceApeProjectile>(PlayerProjectilePoolRef->GetReusableReference());

	if (ProjectileB != nullptr) {
		CheckAndUpdateProjectile(ProjectileB);
		ProjectileB->SetProjectileLocationAndDirection(SpawnLocationB, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
}
