// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "WaveTextRenderActor.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API AWaveTextRenderActor : public ATextRenderActor
{
	GENERATED_BODY()

		FTimerHandle WaveTextTimerHandle;

public:

	//UPROPERTY(ReplicatedUsing = OnRep_TextUpdated)
		FText DisplayText;


	virtual void BeginPlay() override;

	class UTextRenderComponent* WaveTextRenderComponent;



	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	

	void DisplayNextWaveMessage(int _WaveNumber);

private:

	FString WaveTextString;
	int WaveTextStringIndex;


	UFUNCTION(NetMulticast, Reliable)
		void MulticastStartWaveTimer(int _WaveNumber);
	virtual void MulticastStartWaveTimer_Implementation(int _WaveNumber);

	void WaveTextTimerFunc();


};
