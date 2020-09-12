// Fill out your copyright notice in the Description page of Project Settings.

#include "BombLauncher_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/ObjectPoolComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"

UBombLauncher_PWComponent::UBombLauncher_PWComponent() {
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
		MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
		ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
		HitParticlePath = "ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Fire_01.Bomb_Fire_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Hit_01.Bomb_Hit_01'";
		DelayBetweenShots = 0.9f;
		DamageOfProjectiles = 100;
		MovementSpeedOfProjectiles = 900;
		break;

	case EWeaponTier::WT_2:
		MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
		ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
		HitParticlePath = "ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Fire_01.Bomb_Fire_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Hit_01.Bomb_Hit_01'";
		DelayBetweenShots = 0.9f;
		DamageOfProjectiles = 100;
		MovementSpeedOfProjectiles = 900;
		break;

	case EWeaponTier::WT_3:
		MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
		ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
		HitParticlePath = "ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Fire_01.Bomb_Fire_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Hit_01.Bomb_Hit_01'";
		DelayBetweenShots = 0.9f;
		DamageOfProjectiles = 100;
		MovementSpeedOfProjectiles = 900;
		break;

	default:
		MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
		ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
		HitParticlePath = "ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Fire_01.Bomb_Fire_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/Bomb_Hit_01.Bomb_Hit_01'";
		DelayBetweenShots = 0.9f;
		DamageOfProjectiles = 100;
		MovementSpeedOfProjectiles = 900;
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
}

void UBombLauncher_PWComponent::Shoot(FVector _FireDirection) {
	const FRotator FireRotation = _FireDirection.Rotation();

	const FVector GunOffset = FVector(0.f, 0.f, -30.f);

	const FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

	ASpaceApeProjectile* Projectile = Cast<ASpaceApeProjectile>(PlayerProjectilePoolRef->GetReusableReference());

	if (Projectile != nullptr) {

		CheckAndUpdateProjectile(Projectile);
		Projectile->SetProjectileLocationAndDirection(SpawnLocation, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
}
