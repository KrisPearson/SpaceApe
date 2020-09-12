// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerWeaponComponent.h"
#include "GaussBlaster_PWComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API UGaussBlaster_PWComponent : public UPlayerWeaponComponent
{
	GENERATED_BODY()


		UGaussBlaster_PWComponent();


	virtual void Shoot(FVector _FireDirection) override;
	
};
