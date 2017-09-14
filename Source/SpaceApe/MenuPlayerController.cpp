// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "SpaceApe.h"


void AMenuPlayerController::BeginPlayingState() {
	bAutoManageActiveCameraTarget = false;

	FString CameraName = "MainGameCameraActor";

	if (GetWorld()) {
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Access the subclass instance with the * or -> operators.
			if (ActorItr->GetName().Contains(CameraName)) {
				FViewTargetTransitionParams Params;
				SetViewTarget(*ActorItr, Params);
			}
		}
	}
}

