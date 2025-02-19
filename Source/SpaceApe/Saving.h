// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyStructs.h"
#include "Saving.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API USaving : public USaveGame {
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "C++ Variables", meta = (AllowPrivateAccess = true))
		FMyPlayerInfo PlayerSettings;
public:

	FORCEINLINE FMyPlayerInfo GetPlayerStruct() const { return PlayerSettings; }

	FORCEINLINE void SetPlayerStruct(FMyPlayerInfo _NewPlayerInfo) { PlayerSettings = _NewPlayerInfo; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
