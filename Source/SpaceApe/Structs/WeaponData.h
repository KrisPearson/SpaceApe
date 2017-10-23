#pragma once


#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Components/ActorComponent.h"
#include "WeaponData.generated.h"


class UProjectileMovementComponent;
class UStaticMeshComponent;
class UStaticMesh;

/*
THis struct needs to be initialised by each weapon's constructor.
The information here will be used to change weapon effects as well as projectile art.
*/

USTRUCT(BlueprintType)
struct FWeaponData {
	GENERATED_BODY()

		//Projectile Art
		//Necessary components(an array, perhaps ? )

		// type data used to add new components to projectiles. (needed here?)
		//class BaseProjectileComponent ProjectileComponent;

		// The mesh used on the projectile
		UStaticMesh* ProjectileMeshComponent;

	// The Particle System of the projectile
	class UParticleSystem* ProjectileParticleSystem; // SetTemplate() takes UParticleSystem* as argument

													 // The audio effect played when firing a projectile
	USoundBase* FireSound;

	TArray<class UProjectileComponent*> ProjectileComponents;

	// The delay between shots
	float WeaponFireRate;

	// The Damage dealt by the projectiles
	int BaseWeaponDamage;

	// The base movement speed of the projectile
	float BaseProjectileSpeed;




	FWeaponData() {};
	FWeaponData(UStaticMesh* _ProjectileMeshComponent, UParticleSystem* _ProjectileParticleSystem, USoundBase* _FireSound, TArray<class UProjectileComponent*> _ProjectileComponents, float _WeaponFireRate, int _BaseWeaponDamage, float _BaseProjectileSpeed) {
		ProjectileMeshComponent = _ProjectileMeshComponent;
		ProjectileParticleSystem = _ProjectileParticleSystem;
		FireSound = _FireSound;
		ProjectileComponents = _ProjectileComponents;
		WeaponFireRate = _WeaponFireRate;
		BaseWeaponDamage = _BaseWeaponDamage;
		BaseProjectileSpeed = _BaseProjectileSpeed;


	};
};
