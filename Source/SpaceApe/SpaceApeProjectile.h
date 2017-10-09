// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceApeProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHit, AActor*, _Enemy);

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class ASpaceApeProjectile : public AActor
{
	GENERATED_BODY()


		void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float ProjectileDamage = 10.f;

	// The move speed of the projectile. This is used in SetVelocityDirection to set the velocity 
	float CurrentMoveSpeed = 1000;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Projectile Particle System**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ProjectileParticle;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ASpaceApeProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnEnemyHit OnEnemyHit;


	void ToggleEnabled(bool _value);

	void SetProjectileLocationAndDirection(FVector _Loc, FVector _Vel, bool _ToggleEnabled);

	void SetPoolReference(class UObjectPoolComponent* _PoolRef) { OwningPool = _PoolRef; }


protected:

	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetLocationAndVelocityDirection(FVector _Loc, FVector _Vel, bool _ToggleEnabled);
	void MulticastSetLocationAndVelocityDirection_Implementation(FVector _Loc, FVector _Vel, bool _ToggleEnabled);


	/*
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetLocationAndVelocityDirection(FVector _Loc, FVector _Vel, bool _ToggleEnabled);
	void ServerSetLocationAndVelocityDirection_Implementation(FVector _Loc, FVector _Vel, bool _ToggleEnabled);
	bool ServerSetLocationAndVelocityDirection_Validate(FVector _Loc, FVector _Vel, bool _ToggleEnabled);
	*/

	void ResetProjectile();


private:
	class UObjectPoolComponent* OwningPool;

	FTimerHandle ReturnToPoolTimer;

	UWorld* World;

};

