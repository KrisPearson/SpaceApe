// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceApeProjectile.h"
#include "PlayerWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEAPE_API UPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerWeaponComponent();

	void SetObjectPoolReference(class UObjectPoolComponent* _PoolRef);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//class ASpaceApeProjectile ProjectileToSpawn;

	TSubclassOf<class ASpaceApeProjectile> ProjectileToSpawn = ASpaceApeProjectile::StaticClass();


	class ASpaceApePlayerCharacter* OwningCharacter;

	UWorld* World;

	// A reference to the character's projectile object pool
	class UObjectPoolComponent* PlayerProjectilePoolRef;




public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Shoot(FVector _FireDirection);

	float WeaponFireRate = 0.14f;


	UObjectPoolComponent* GetObjectPoolReference();



	
};
