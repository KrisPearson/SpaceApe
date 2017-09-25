// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_GetClosestPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyAllTypes.h"
#include "SpaceApePlayerCharacter.h"
#include "EngineUtils.h"

#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

//#include "AIEssentialsCharacter.h"

#include "Engine/World.h"
#include "Engine.h" 
#include "Enemy_AIController.h"

UBTService_GetClosestPlayer::UBTService_GetClosestPlayer() {
	bCreateNodeInstance = true;
}

void UBTService_GetClosestPlayer::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) {

	if (AEnemy_AIController* EnemyController = Cast<AEnemy_AIController>(OwnerComp.GetOwner())) {

		float distance = 99999;
		APawn* returnPawn = nullptr;

		FVector enemyLoc = EnemyController->GetPawn()->GetActorLocation();
	
		for (TActorIterator<ASpaceApePlayerCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			float newDistance = FVector::Dist(ActorItr->GetActorLocation(), enemyLoc);
			if (newDistance < distance) {
				distance = newDistance;
				returnPawn = Cast<APawn>(*ActorItr);
			}
		}
		if (returnPawn ) OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyController->EnemyKeyID, returnPawn);
	}
	
}
