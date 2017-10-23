// Fill out your copyright notice in the Description page of Project Settings.

#include "BeamRifle_PWComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ObjectPoolComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/ConstructorHelpers.h"


UBeamRifle_PWComponent::UBeamRifle_PWComponent() {
	UE_LOG(LogTemp, Warning, TEXT(" UBeamRifle_PWComponent Constructor"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("StaticMesh'/Game/TwinStick/Meshes/BeamColissionMesh.BeamColissionMesh'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("ParticleSystem'/Game/Particles/TrailBeam_Particle.TrailBeam_Particle'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/RailDriver.RailDriver'"));


	TArray<class UProjectileComponent*> ProjectileComponentArray;


	// could perform a level check here to alter the weapon data

	WeaponData = FWeaponData(
		ProjectileMeshAsset.Object,
		ProjectileParticleAsset.Object,
		FireAudio.Object,
		ProjectileComponentArray,
		1.5f,
		400,
		9500
	);


	BeamSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Beam Spline Component"));
	BeamSpline->bSplineHasBeenEdited = true;


	PrimaryComponentTick.bCanEverTick = false;
	//SetComponentTickEnabled(true);
}

void UBeamRifle_PWComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {

	UE_LOG(LogTemp, Warning, TEXT(" UBeamRifle_PWComponent  Tick"));
	// if (beamrpjectiles ! empty) 
	//for beamprojectiles
	//move each along spline -time between launch

	//clear beam projectiles at end of spline? (what if it is a refraction beam? A:add more points to spline and keep check to update spline points)

	if (CurrentBeamProjectiles.Num() > 0) {
		for (int i = 0; i < CurrentBeamProjectiles.Num(); i++) {
			CurrentBeamProjectiles[i]->SetActorLocation(BeamSpline->GetLocationAtTime(ShootTime += DeltaTime, ESplineCoordinateSpace::Local, true));
			//shoot time plus delta time
		}

	}

}

void UBeamRifle_PWComponent::Shoot(FVector _FireDirection) {
	FireDirection = _FireDirection;

	FireRotation = FireDirection.Rotation();

	FVector GunOffset = FVector(0.f, 0.f, -30.f);

	SpawnLocationForbeam = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

	LoopIndex = 0;
	GetWorld()->GetTimerManager().SetTimer(FireLoopTimerHandle, this, &UBeamRifle_PWComponent::FireLoop, TimeBetweenProjectiles, true);
}

void UBeamRifle_PWComponent::FireLoop() {
	if (BeamSpline != nullptr) {

		TArray<FVector> SplinePoints;
		SplinePoints.Add(SpawnLocationForbeam);

		SplinePoints.Add(FVector(600, 600, 0));

		SplinePoints.Add(FVector(1000, 1000, 0));

		BeamSpline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Local, true);
		BeamSpline->SetTangentAtSplinePoint(1, FVector(1000, 7000, 0), ESplineCoordinateSpace::Local, true);
	}


	//const FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

	ASpaceApeProjectile* Projectile = Cast<ASpaceApeProjectile>(PlayerProjectilePoolRef->GetReusableReference());

	if (Projectile != nullptr) {
		//Projectile->ToggleEnabled(true); // moved to the projectile's multicast. If needed, then a bool param could be added to the multicast method.


		Projectile->SetProjectileLocationAndDirection(SpawnLocationForbeam, FireDirection, true); // still want to do this for the beam?

																							 //Projectile->GetParticleComponent()->SetBeamSourcePoint(1, SpawnLocation, 1);
																							 //Projectile->GetParticleComponent()->SetBeamTargetPoint(1, SpawnLocation * _FireDirection, 1);

																							 //UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.ProjectileParticleSystem, SpawnLocation, FRotator::ZeroRotator, false);

																							 //if (beam)
																							 //{
																							 //	beam->SetBeamSourcePoint(1, SpawnLocation, 1);
																							 //	beam->SetBeamTargetPoint(1, (SpawnLocation +_FireDirection * 500), 1);
																							 //}
		CurrentBeamProjectiles.Add(Projectile); //need to consider order they are added in order to correctly adjust time input
		// tell component on projectile to follow spline?


		ShootTime = GetWorld()->GetTimeSeconds();

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
		return;
	}
	LoopIndex++;
	if (LoopIndex >= NumberOfProjectiles) {
		UE_LOG(LogTemp, Warning, TEXT("LoopIndex >= NumberOfProjectiles"));
		//GetWorld()->GetTimerManager().SetTimer(FireLoopTimerHandle, this, &UBeamRifle_PWComponent::FireLoop, TimeBetweenProjectiles, false);
	
		GetWorld()->GetTimerManager().ClearTimer(FireLoopTimerHandle);
	}
}
