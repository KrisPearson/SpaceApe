// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPWComponent.h"
//#include "SpaceApeProjectile.h"
#include "SpaceApePlayerCharacter.h"

void ULaserPWComponent::Shoot(FVector _FireDirection) {
	//UE_LOG(LogTemp, Warning, TEXT("Laser UPlayerWeaponComponent firing"));

	const FRotator FireRotation = _FireDirection.Rotation();

	FVector GunOffset = FVector(0.f, 0.f, -30.f);

	const FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

//	ASpaceApeProjectile* NewProjectile = GetWorld()->SpawnActor<ASpaceApeProjectile>(ProjectileToSpawn, SpawnLocation, FireRotation);

	//ASpaceApeProjectile* NewProjectile = GetWorld()->SpawnActor<ASpaceApeProjectile>();

	ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);
	if (OwningCharacter != nullptr && World != nullptr) {
		NewProjectile->OnEnemyHit.AddDynamic(OwningCharacter, &ASpaceApePlayerCharacter::DealDamage);
		//NewProjectile->OnEnemyHit.AddDynamic(this, &ASpaceApePlayerCharacter::DealDamage);
	}
}


/*
void ULaserPWComponent::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Laser ULaserPWComponent spawned"));


}
*/
