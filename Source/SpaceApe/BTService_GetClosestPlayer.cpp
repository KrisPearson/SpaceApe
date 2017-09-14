// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_GetClosestPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyAllTypes.h"
#include "SpaceApePawn.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

//#include "AIEssentialsCharacter.h"

#include "Engine/World.h"
#include "Enemy_AIController.h"

UBTService_GetClosestPlayer::UBTService_GetClosestPlayer() {
	bCreateNodeInstance = true;
}

void UBTService_GetClosestPlayer::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) {


	//this is a likely cause of lag spikes. Needs investigation.
	//possible cause of lag on colission with player = enemy searches for target repeatedly. Not yet confirmed.

	

	//better alternative = ask gamestate for closest player, sending this actor's position as param

		/*

		for (TObjectIterator<UClass> It; It; ++It)
		{
			if (It->IsChildOf(ASpaceApePawn::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
			{
				//LocatedPlayers.Add(*It);
				
			}
		}
		*/


		
	if (AEnemy_AIController* EnemyController = Cast<AEnemy_AIController>(OwnerComp.GetOwner())) {



		APawn* PlayerPawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn()); // <==should check for closest
		if (PlayerPawn) {
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyController->EnemyKeyID, PlayerPawn);
		}
	}
	
}
