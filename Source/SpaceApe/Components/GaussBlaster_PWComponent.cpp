// Fill out your copyright notice in the Description page of Project Settings.

#include "GaussBlaster_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/ObjectPoolComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"

UGaussBlaster_PWComponent::UGaussBlaster_PWComponent() {
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'"));
	//static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'"));

	UE_LOG(LogTemp, Warning, TEXT(" UGaussBlaster_PWComponent Constructor"));



	//if (OwningCharacter) {
		//WeaponTier = OwningCharacter->GetCurrentWeaponTier();

		TArray<class UProjectileComponent*> ProjectileComponentArray;
		FString MeshPath;
		FString ProjectileParticlePath;
		FString HitParticlePath;
		FString FireAudioPath;
		FString HitAudioPath;
		float DelayBetweenShots;
		int DamageOfProjectiles;
		float MovementSpeedOfProjectiles;

		switch (WeaponTier) {
		case EWeaponTier::WT_1:
			MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
			ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'";
			HitParticlePath = "";
			FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'";
			HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussHit_01.GaussHit_01'";
			DelayBetweenShots = 0.185f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 1200;
			break;
		case EWeaponTier::WT_2:
			MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
			ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'";
			HitParticlePath = "";
			FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'";
			HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussHit_01.GaussHit_01'";
			DelayBetweenShots = 0.185f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 1200;
			break;

		case EWeaponTier::WT_3:
			MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
			ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'";
			HitParticlePath = "";
			FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'";
			HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussHit_01.GaussHit_01'";
			DelayBetweenShots = 0.185f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 1200;
			break;

		default:
			MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
			ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/Blaster_Particle.Blaster_Particle'";
			HitParticlePath = "";
			FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussFire_01.GaussFire_01'";
			HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/GaussHit_01.GaussHit_01'";
			DelayBetweenShots = 0.185f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 1200;
			break;
		}


		static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(*MeshPath);
		static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(*ProjectileParticlePath);
		static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleAsset(*HitParticlePath);
		static ConstructorHelpers::FObjectFinder<USoundBase> FireAudioAsset(*FireAudioPath);
		static ConstructorHelpers::FObjectFinder<USoundBase> HitAudioAsset(*HitAudioPath);

		WeaponData = FWeaponData(
			ProjectileComponentArray,
			ProjectileMeshAsset.Object,
			ProjectileParticleAsset.Object,
			HitParticleAsset.Object,
			FireAudioAsset.Object,
			HitAudioAsset.Object,
			DelayBetweenShots,
			DamageOfProjectiles,
			MovementSpeedOfProjectiles
		);

	//}
}




	/*
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
	*/

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
