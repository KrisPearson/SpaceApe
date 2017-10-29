// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "WeaponPickup.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponPickupType : uint8 {
	WP_GaussBlaster 	UMETA(DisplayName = "GaussBlaster"),
	WP_MissileLauncher	UMETA(DisplayName = "MissileLauncher"),
	WP_BeamRifle		UMETA(DisplayName = "BeamRifle"),
	WP_BombPitcher		UMETA(DisplayName = "BombPitcher")
};


/**
 * 
 */
UCLASS()
class SPACEAPE_API AWeaponPickup : public ABasePickup
{
	GENERATED_BODY()

		AWeaponPickup();

protected:

	/** Overridable function to do something when overlapped*/
	virtual void OnBeginOverlapAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPlayerWeaponComponent> WeaponType;

	virtual void AssignRandomPickupType() override;

private:

	EWeaponPickupType WeaponPickupTypeEnum;
	
};
