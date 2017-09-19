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

public:

	UPROPERTY(ReplicatedUsing = OnRep_TextUpdated)
		FText DisplayText;


	virtual void BeginPlay() override;

	class UTextRenderComponent* WaveTextRenderComponent;

	UFUNCTION()
		virtual void OnRep_TextUpdated();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
