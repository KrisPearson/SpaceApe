// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
#include "SpaceApeProjectile.h"
#include "Structs/WeaponData.h"
#include "PlayerWeaponComponent.generated.h"



class UObjectPoolComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEAPE_API UPlayerWeaponComponent : public UActorComponent 
{
	GENERATED_BODY()



public:
	// Sets default values for this component's properties
	UPlayerWeaponComponent();

	void SetObjectPoolReference(class UObjectPoolComponent* _PoolRef);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Shoot(FVector _FireDirection);

	const float WeaponFireRate = 0.14f;

	UObjectPoolComponent* GetObjectPoolReference();





protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FWeaponData WeaponData;

	//class ASpaceApeProjectile ProjectileToSpawn;

	TSubclassOf<class ASpaceApeProjectile> ProjectileToSpawn = ASpaceApeProjectile::StaticClass();


	class ASpaceApePlayerCharacter* OwningCharacter;

	UWorld* World;

	// A reference to the character's projectile object pool
	UObjectPoolComponent* PlayerProjectilePoolRef;

	void CheckAndUpdateProjectile(class ASpaceApeProjectile* _Projectile);


public:

	float GetFireRate() { return WeaponData.WeaponFireRate; }

	USoundBase* GetFireSound() { return WeaponData.FireSound;  }


	FWeaponData GetWeaponData() { return WeaponData;  }

private:

	//FTimerHandle ShootDelayTimer;

};


