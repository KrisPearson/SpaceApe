// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_SpaceApe.h"
#include "Engine.h"
#include "EngineUtils.h"

APlayerController_SpaceApe::APlayerController_SpaceApe() {
	bAutoManageActiveCameraTarget = false;

	FString CameraName = "MainGameCameraActor";

	if (GetWorld()) {
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Access the subclass instance with the * or -> operators.
			if (ActorItr->GetName().Contains(CameraName)) {
				FViewTargetTransitionParams Params;
				SetViewTarget(*ActorItr, Params);
				UE_LOG(LogTemp, Warning, TEXT("Construct- SetViewTarget"));
			}
		}
	}

}

void APlayerController_SpaceApe::BeginPlayingState() {
	FString CameraName = "MainGameCameraActor";

	if (GetWorld()) {
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Access the subclass instance with the * or -> operators.
			if (ActorItr->GetName().Contains(CameraName)) {
				FViewTargetTransitionParams Params;
				SetViewTarget(*ActorItr, Params);
				UE_LOG(LogTemp, Warning, TEXT("BeginPlayingState - SetViewTarget"));
			}
		}
	}
}

/*

void APlayerController_SpaceApe::Possess(APAwn * InPawn) {
	Super::Possess(InPawn);
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &APlayerController_SpaceApe::SetCameraViewToMainCam, 0.5f, false);
}
*/

void APlayerController_SpaceApe::SetCameraViewToMainCam() {
	FString CameraName = "MainGameCameraActor";

	if (GetWorld()) {
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Access the subclass instance with the * or -> operators.
			if (ActorItr->GetName().Contains(CameraName)) {
				FViewTargetTransitionParams Params;
				SetViewTarget(*ActorItr, Params);
				UE_LOG(LogTemp, Warning, TEXT("Late SetViewTarget "));
			}
		}
	}
}
