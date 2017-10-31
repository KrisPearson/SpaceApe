// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerWeaponComponent.h"
#include "Components/ObjectPoolComponent.h"
#include "Particles/ParticleSystem.h"
//#include "Components/PlayerWeaponComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"


/*
The Player Weapon Component dictates:
	> the Projectile type
	> The Spawn pattern
	> The rate of fire
*/



// Sets default values for this component's properties
UPlayerWeaponComponent::UPlayerWeaponComponent() {

	UE_LOG(LogTemp, Warning, TEXT(" UPlayerWeaponComponent Constructor"));

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	World = GetWorld();
	OwningCharacter = Cast<ASpaceApePlayerCharacter>(GetOwner());


	//ProjectileToSpawn = ASpaceApeProjectile();
	// ...

	/*

	TODO:
	The base class shouldn't have the loading code.
	Needs replacing with Tier 1/2/3 weaponcomponents.
	This constructor should just assign character and tier values for derived constructors
	
	*/

	if (OwningCharacter) {
		WeaponTier = OwningCharacter->GetCurrentWeaponTier();

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
			HitParticlePath = "";
			FireAudioPath = "/Game/TwinStick/Audio/KKIIDDZZ_00018";
			HitAudioPath = "";
			DelayBetweenShots = 0.2f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 900;
			break;

		case EWeaponTier::WT_2:
			MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
			ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
			HitParticlePath = "";
			FireAudioPath = "/Game/TwinStick/Audio/KKIIDDZZ_00018";
			HitAudioPath = "";
			DelayBetweenShots = 0.2f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 900;
			break;

		case EWeaponTier::WT_3:
			MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
			ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
			HitParticlePath = "";
			FireAudioPath = "/Game/TwinStick/Audio/KKIIDDZZ_00018";
			HitAudioPath = "";
			DelayBetweenShots = 0.2f;
			DamageOfProjectiles = 40;
			MovementSpeedOfProjectiles = 900;
			break;

		default :
			MeshPath = "/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile";
			ProjectileParticlePath = "/Game/Particles/Blaster_Particle";
			HitParticlePath = "";
			FireAudioPath = "/Game/TwinStick/Audio/KKIIDDZZ_00018";
			HitAudioPath = "";
			DelayBetweenShots = 0.2f;
			DamageOfProjectiles = 40;
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

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ProjectileParticleAsset(TEXT("/Game/Particles/Blaster_Particle"));
	//static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/KKIIDDZZ_00018"));

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

