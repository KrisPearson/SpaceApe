// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyAllTypes.h"
#include "Enemy.h"

AEnemy_AIController::AEnemy_AIController() {
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

}

void AEnemy_AIController::Possess(APawn * InPawn) {
	Super::Possess(InPawn);

	AEnemy* Enemy = Cast<AEnemy>(InPawn);

	if (Enemy && Enemy->BotBehavior) {
		BlackBoardComp->InitializeBlackboard(*Enemy->BotBehavior->BlackboardAsset);

		EnemyKeyID = BlackBoardComp->GetKeyID("Target");

		BehaviorComp->StartTree(*Enemy->BotBehavior);
	}
}
