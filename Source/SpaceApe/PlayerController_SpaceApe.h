// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_SpaceApe.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API APlayerController_SpaceApe : public APlayerController
{
	GENERATED_BODY()

		APlayerController_SpaceApe();

	//void Possess(APawn * InPawn) override;
	
	void SetCameraViewToMainCam();

	void BeginPlayingState() override;
};
