// Fill out your copyright notice in the Description page of Project Settings.

#include "MissileLauncher_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/PlayerWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"

UMissileLauncher_PWComponent::UMissileLauncher_PWComponent() {

	UE_LOG(LogTemp, Warning, TEXT(" UMissileLauncher_PWComponent Constructor"));

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
		ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/MissileTrail_Particle.MissileTrail_Particle'";
		HitParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/BeamHit_Particle.BeamHit_Particle'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileLaunch_01.MissileLaunch_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileHit_01.MissileHit_01'";
		DelayBetweenShots = 0.5f;
		DamageOfProjectiles = 80;
		MovementSpeedOfProjectiles = 1800;
		break;
	case EWeaponTier::WT_2:
		MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
		ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/MissileTrail_Particle.MissileTrail_Particle'";
		HitParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/BeamHit_Particle.BeamHit_Particle'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileLaunch_01.MissileLaunch_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileHit_01.MissileHit_01'";
		DelayBetweenShots = 0.5f;
		DamageOfProjectiles = 80;
		MovementSpeedOfProjectiles = 1800;
		break;

	case EWeaponTier::WT_3:
		MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
		ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/MissileTrail_Particle.MissileTrail_Particle'";
		HitParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/BeamHit_Particle.BeamHit_Particle'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileLaunch_01.MissileLaunch_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileHit_01.MissileHit_01'";
		DelayBetweenShots = 0.5f;
		DamageOfProjectiles = 80;
		MovementSpeedOfProjectiles = 1800;
		break;

	default:
		MeshPath = "StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'";
		ProjectileParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/MissileTrail_Particle.MissileTrail_Particle'";
		HitParticlePath = "ParticleSystem'/Game/Particles/WeaponParticles/BeamHit_Particle.BeamHit_Particle'";
		FireAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileLaunch_01.MissileLaunch_01'";
		HitAudioPath = "SoundWave'/Game/Audio/WeaponSounds/MissileHit_01.MissileHit_01'";
		DelayBetweenShots = 0.5f;
		DamageOfProjectiles = 80;
		MovementSpeedOfProjectiles = 1800;
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


//	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
//	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("ParticleSystem'/Game/Particles/Blaster_ParticleCopy.Blaster_ParticleCopy'"));
//	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));


//	TArray<class UProjectileComponent*> ProjectileComponentArray;
	/*
	WeaponData = FWeaponData(
		ProjectileComponentArray, // Components to be added to the projectile actor
		ProjectileMeshAsset.Object, // Mesh used for collision events and visual appearance
		ProjectileParticleAsset.Object, // The constant visual effect particle ( for trails etc)
		nullptr,
		FireAudio.Object, // Sound effect played when fired
		nullptr,
		0.9f, // Delay between shots
		200, // Damage of projectiles
		1800 // Movement Speed of projectiles
	);
	*/
}
