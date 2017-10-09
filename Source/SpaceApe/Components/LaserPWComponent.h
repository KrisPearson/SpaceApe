// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerWeaponComponent.h"
#include "LaserPWComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API ULaserPWComponent : public UPlayerWeaponComponent
{
	GENERATED_BODY()


	void Shoot(FVector _FireDirection) override;
	

	//void BeginPlay() override;
};
