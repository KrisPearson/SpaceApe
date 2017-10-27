// Fill out your copyright notice in the Description page of Project Settings.

#include "MissileLauncher_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"

UMissileLauncher_PWComponent::UMissileLauncher_PWComponent() {


	UE_LOG(LogTemp, Warning, TEXT(" UMissileLauncher_PWComponent Constructor"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("ParticleSystem'/Game/Particles/Blaster_ParticleCopy.Blaster_ParticleCopy'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));


	TArray<class UProjectileComponent*> ProjectileComponentArray;

	WeaponData = FWeaponData(
		ProjectileComponentArray, // Components to be added to the projectile actor
		ProjectileMeshAsset.Object, // Mesh used for collision events and visual appearance
		ProjectileParticleAsset.Object, // The constant visual effect particle ( for trails etc)
		nullptr,
		FireAudio.Object, // Sound effect played when fired
		nullptr,
		1.f, // Delay between shots
		80, // Damage of projectiles
		800 // Movement Speed of projectiles
	);
}
