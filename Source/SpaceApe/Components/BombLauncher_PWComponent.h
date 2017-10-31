// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerWeaponComponent.h"
#include "BombLauncher_PWComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API UBombLauncher_PWComponent : public UPlayerWeaponComponent
{
	GENERATED_BODY()

		UBombLauncher_PWComponent();
	
		virtual void Shoot(FVector _FireDirection) override;
	
};
