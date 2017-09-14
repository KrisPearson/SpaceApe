// Copyright 1998-2017 Epic Games, Inc. All Rights Reserve

#include "SpaceApeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy.h"

ASpaceApeProjectile::ASpaceApeProjectile() 
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("/Game/Particles/Blaster_Particle"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASpaceApeProjectile::OnHit);		// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	ProjectileMesh->SetVisibility(0);


	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	ProjectileParticle->SetTemplate(ProjectileParticleAsset.Object);
	ProjectileParticle->SetupAttachment(RootComponent);

	ProjectileParticle->EmitterDelay = 0.0f;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = true;
	bReplicateMovement = true;

}

void ASpaceApeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OnEnemyHit.IsBound())
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor"));
		OnEnemyHit.Broadcast(OtherActor);
	}

	/*

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		if (OtherActor->IsA(AEnemy::StaticClass())) {
			Cast<AEnemy>(OtherActor)->ReceiveDamage(ProjectileDamage);
			UE_LOG(LogTemp, Log, TEXT("HIT ENEMY"));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("Hit Something else"));
		}
	}
	*/



	// Only add impulse and destroy projectile if we hit a physics

	/*
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	*/

	Destroy();
}