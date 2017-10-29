// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerWeaponComponent.h"
#include "Components/ObjectPoolComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/PlayerWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"


/*
The Player Weapon Component dictates:
	> the Projectile type
	> The Spawn pattern
	> The rate of fire
*/



// Sets default values for this component's properties
UPlayerWeaponComponent::UPlayerWeaponComponent()
{


	UE_LOG(LogTemp, Warning, TEXT(" UPlayerWeaponComponent Constructor"));

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	World = GetWorld();
	OwningCharacter = Cast<ASpaceApePlayerCharacter>(GetOwner());


	//ProjectileToSpawn = ASpaceApeProjectile();
	// ...

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("/Game/Particles/Blaster_Particle"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/KKIIDDZZ_00018"));



	TArray<class UProjectileComponent*> ProjectileComponentArray;



	WeaponData = FWeaponData(
		ProjectileComponentArray, // Components to be added to the projectile actor
		ProjectileMeshAsset.Object, // Mesh used for collision events and visual appearance
		ProjectileParticleAsset.Object, // The constant visual effect particle ( for trails etc)
		nullptr,
		FireAudio.Object, // Sound effect played when fired
		nullptr,
		0.2f, // Delay between shots
		40, // Damage of projectiles
		500 // Movement Speed of projectiles
	);
}


void UPlayerWeaponComponent::SetObjectPoolReference(UObjectPoolComponent* _PoolRef) {
	PlayerProjectilePoolRef = _PoolRef;
}

// Called when the game starts
void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	OwningCharacter = Cast<ASpaceApePlayerCharacter>(GetOwner());

	UE_LOG(LogTemp, Warning, TEXT("Base UPlayerWeaponComponent BeginPlay"));
}



// Called every frame
void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/*
This overridable method gets projectiles from the character's object pool component and "spawns" them. 
should be called by the server by an RPC when networking.
*/
void UPlayerWeaponComponent::Shoot(FVector _FireDirection) {
	
	const FRotator FireRotation = _FireDirection.Rotation();

	FVector GunOffset = FVector(0.f, 0.f, -30.f);

	const FVector SpawnLocation = GetOwner()->GetActorLocation() + FireRotation.RotateVector(GunOffset);

	ASpaceApeProjectile* Projectile = Cast<ASpaceApeProjectile>( PlayerProjectilePoolRef->GetReusableReference() );

	

	if (Projectile != nullptr) { 
		//Projectile->ToggleEnabled(true); // moved to the projectile's multicast. If needed, then a bool param could be added to the multicast method.
		CheckAndUpdateProjectile(Projectile);
		Projectile->SetProjectileLocationAndDirection(SpawnLocation, _FireDirection, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile is nullptr. Either cast failed, or no projectile was returned from the Object Pool."));
	}


//	for (int i = 0; i < PlayerProjectilePoolRef->Num(); i++) {
//		&PlayerProjectilePoolRef[i]->
//	}


	//GetProjecectileandenable
	//PlayerProjectilePoolRef

	//ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(ProjectileToSpawn, SpawnLocation, FireRotation);
	//ASpaceApeProjectile* NewProjectile = World->SpawnActor<ASpaceApeProjectile>(SpawnLocation, FireRotation);
	//NewProjectile->OnEnemyHit.AddDynamic(Cast<ASpaceApePlayerCharacter>(OwningActor), &ASpaceApePlayerCharacter::DealDamage);

}

UObjectPoolComponent * UPlayerWeaponComponent::GetObjectPoolReference() {
	if (PlayerProjectilePoolRef != nullptr) {
		return PlayerProjectilePoolRef;
	}
	else return nullptr;
}

void UPlayerWeaponComponent::CheckAndUpdateProjectile(ASpaceApeProjectile * _Projectile) {
	// need an RPC for this?
	if (_Projectile->GetWeaponDataID() != GetUniqueID()) {
		// update the weapon data of the projectile, and update the id
		_Projectile->PassNewWeaponData(WeaponData, GetUniqueID());

	}
}

