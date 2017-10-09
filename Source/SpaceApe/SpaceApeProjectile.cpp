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
#include "Components/ObjectPoolComponent.h"
#include "Enemy.h"

void ASpaceApeProjectile::BeginPlay() {
	Super::BeginPlay();

	World = GetWorld();
}

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
	//ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::)
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASpaceApeProjectile::OnHit);		// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	//ProjectileMesh->SetVisibility(0);


	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	ProjectileParticle->SetTemplate(ProjectileParticleAsset.Object);
	ProjectileParticle->SetupAttachment(RootComponent);

	ProjectileParticle->EmitterDelay = 0.0f;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	ProjectileMovement->bShouldBounce = false; 
	ProjectileMovement->Bounciness = 1.0f; // Do not lose velocity when bounces.

	ProjectileMovement->SetUpdatedComponent(GetRootComponent()); // Ensures that the root component is updated by the projectile movement component

	//ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;

	bReplicates = true;
	//bReplicateMovement = true;

	ToggleEnabled(false);

	CurrentMoveSpeed = 1500;

	UE_LOG(LogTemp, Log, TEXT(" ASpaceApeProjectile constructor"));
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

	//Destroy();
	if (OwningPool != NULL) {
		SetProjectileLocationAndDirection(FVector(0, 0, 0), FVector(0, 0, 0), false); // this doesn't appear to be working
		OwningPool->ReturnReusableReference(this);
	}
}

/*
Enables and disabled the projectile. Used for getting it from and returning it too the object pool and
when restarting the projectile following a collission or other similar event.
In order to enable the projectile over the network, this method should  be called before applying other changes.
*/
void ASpaceApeProjectile::ToggleEnabled(bool _value) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" ToggleEnabled. Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));

	//ProjectileMesh->SetVisibility(_value); // need some kind of check to see whether we want this or not
	if (_value) { // enable the projectile

		UE_LOG(LogTemp, Log, TEXT(" Toggle enabled true"));

		ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ProjectileParticle->ActivateSystem(true);
		ProjectileMovement->Activate();
		ProjectileMovement->SetUpdatedComponent(GetRootComponent()); // moved to constructor

		World->GetTimerManager().SetTimer(ReturnToPoolTimer, this, &ASpaceApeProjectile::ResetProjectile, 6.f);

	}
	else { // disable the projectile
		
		UE_LOG(LogTemp, Log, TEXT(" Toggle enabled false"));

		ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileParticle->DeactivateSystem();// Deactivate the particle. It may be the case that the particle will reamin if lifetime = 0. If so, check KillOnDeactivate in the particle system.
		ProjectileMovement->Deactivate(); 

	}
}

/*
Updates the location and velocity of the projectile.
This is the intended method for firing the projectile from the weapon component.
*/
void ASpaceApeProjectile::SetProjectileLocationAndDirection(FVector _Loc, FVector _Vel, bool _ToggleEnabled) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" SetProjectileLocationAndDirection. Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));
	if (Role == ROLE_Authority) {
		MulticastSetLocationAndVelocityDirection(_Loc, _Vel, _ToggleEnabled);
	}
	//else {
		//ServerSetLocationAndVelocityDirection(_Loc, _Vel, _ToggleEnabled); // shouldn't need this?
	//}
}


/*
void ASpaceApeProjectile::ServerSetLocationAndVelocityDirection_Implementation(FVector _Loc, FVector _Vel, bool _ToggleEnabled) {
	UE_LOG(LogTemp, Log, TEXT(" ServerSetLocationAndVelocityDirection"));
	
	//ToggleEnabled(_ToggleEnabled); // re-enable movement component and restart particle system.
	//SetActorLocation(_Loc);	// set the location of the projectile on the client(s) and server
	//ProjectileMovement->Velocity = FVector(0, 0, 0);
	//ProjectileMovement->SetVelocityInLocalSpace(_Vel * CurrentMoveSpeed); // Apply velocity in fire direction
	
	MulticastSetLocationAndVelocityDirection(_Loc, _Vel, _ToggleEnabled);
}
*/

/*
bool ASpaceApeProjectile::ServerSetLocationAndVelocityDirection_Validate(FVector _Loc, FVector _Vel, bool _ToggleEnabled) {
	return true;
}
*/

/*
Updates the projectiles location and velocity across the network.
*/
void ASpaceApeProjectile::MulticastSetLocationAndVelocityDirection_Implementation(FVector _Loc, FVector _Vel, bool _ToggleEnabled) {
	ToggleEnabled(_ToggleEnabled); // re-enable movement component and restart particle system.
	SetActorLocation(_Loc);	// set the location of the projectile on the client(s) and server
	ProjectileMovement->Velocity = _Vel * CurrentMoveSpeed;
	
	//ProjectileMovement->Velocity = FVector(0, 0, 0);
	//ProjectileMovement->SetVelocityInLocalSpace(_Vel * CurrentMoveSpeed); // Apply velocity in fire direction	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" MulticastSetLocationAndVelocityDirection_Implementation. Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));
	//UE_LOG(LogTemp, Log, TEXT(" SetVelocityInLocalSpace %f %f %f"), _Vel.X, _Vel.Y, _Vel.Z);

}

void ASpaceApeProjectile::ResetProjectile() {
	if (OwningPool != NULL) {
		SetProjectileLocationAndDirection(FVector(0, 0, 0), FVector(0, 0, 0), false); // this doesn't appear to be working
		OwningPool->ReturnReusableReference(this);
	}
}

/* 
Updates the velocity o the projectile on cliets. Called whenever a change is made to the velocity.
Only works with bReplicateMovement.....
*/
void  ASpaceApeProjectile::PostNetReceiveVelocity(const FVector& NewVelocity) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" ToggleEnabled. Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));
	
	UE_LOG(LogTemp, Log, TEXT(" PostNetReceiveVelocity %f %f %f"), NewVelocity.X, NewVelocity.Y, NewVelocity.Z);
	ProjectileMovement->Velocity = NewVelocity;
}