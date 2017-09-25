// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyAllTypes.h"
#include "Enemy_AIController.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {

	AEnemy_AIController* EnemyController = Cast <AEnemy_AIController>(OwnerComp.GetAIOwner());

	APawn* PlayerPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyController->EnemyKeyID));
	if (PlayerPawn) {
		//EnemyController->MoveToActor(PlayerPawn, 5.f, true, true, true, 0, true);
		EnemyController->MoveToLocation(PlayerPawn->GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "I SEE THE PLAYER");
		//UE_LOG(LogTemp, Log, TEXT("Your message %f"), PlayerPawn->GetActorLocation().X );
		return EBTNodeResult::Succeeded;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "FAILED");
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Failed;
	
}
