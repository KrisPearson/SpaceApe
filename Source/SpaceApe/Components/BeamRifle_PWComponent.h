// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerWeaponComponent.h"
#include "BeamRifle_PWComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API UBeamRifle_PWComponent : public UPlayerWeaponComponent
{
	GENERATED_BODY()
	
	
		UBeamRifle_PWComponent();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	FTimerHandle FireLoopTimerHandle;

	virtual void Shoot(FVector _FireDirection) override;
	
protected:

	virtual void FireLoop();

	int NumberOfProjectiles = 5;

	float TimeBetweenProjectiles = 0.035f;

	UPROPERTY(EditAnywhere, Category = YourCategory)
		class USplineComponent* BeamSpline;

	// TSubobjectPtr<class USplineComponent> YourSplineComponent;

private:
	FVector FireDirection;
	
	FRotator FireRotation;

	FVector SpawnLocationForbeam;


	int LoopIndex = 0;

	TArray<class ASpaceApeProjectile*> CurrentBeamProjectiles; // how best to link time between shots to each projectile?

	

	float ShootTime;


};
