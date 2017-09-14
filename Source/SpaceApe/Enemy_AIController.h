// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

		UPROPERTY(transient)
		class UBlackboardComponent* BlackBoardComp;
	
	//UPROPERTY(transient)
		class UBehaviorTreeComponent* BehaviorComp;

public:

	AEnemy_AIController();

	virtual void Possess(APawn* InPawn);

	uint8 EnemyKeyID;
};
