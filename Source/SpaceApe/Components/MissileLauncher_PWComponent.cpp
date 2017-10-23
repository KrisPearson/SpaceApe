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
		ProjectileMeshAsset.Object,
		ProjectileParticleAsset.Object,
		FireAudio.Object,
		ProjectileComponentArray,
		1.f,
		80,
		800
	);
}
