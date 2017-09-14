// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Enemy_AIController.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "BehaviorTree/BehaviorTree.h"




// Sets default values
AEnemy::AEnemy()  {

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	FloatingMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement Component"));

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh"));
	EnemyMesh->SetupAttachment(RootComponent);

	FloatingMovementComponent->MaxSpeed = MaxSpeed;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



//	AEnemy_AIController* Controller = new AEnemy_AIController();
//	AIControllerClass = Controller;

	/*  Causes fatal error on build
	UObject* something;


		something = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/AI/EnemyAI_BP"));
		if (something != nullptr) {
			UBlueprint* bp = Cast<UBlueprint>(something);
			TSubclassOf<class UObject> MyItemBlueprint;
			MyItemBlueprint = (UClass*)bp->GeneratedClass;

			AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
			AIControllerClass = MyItemBlueprint;
		}
*/
	
}

void AEnemy::BeginPlay() {
		Super::BeginPlay();


		if (BaseMat != nullptr) {
			DynamicEnemyMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
			EnemyMesh->SetMaterial(0, DynamicEnemyMaterial);			
		}
}


bool AEnemy::ReceiveDamage(int DamageAmount) {
	HealthPoints -= DamageAmount;

	if (!CheckIfAlive()) {
		EnemyDeath();
		return true;
	}
	else if (DynamicEnemyMaterial) {
		DynamicEnemyMaterial->SetScalarParameterValue(FName("DamageFlashScaler"), 0.5f);
		GetWorld()->GetTimerManager().SetTimer(DamageFlashTimerHandle, this, &AEnemy::DisableMaterialFlash, 0.2f, false);

		return false;
	}

	return false;

}

void AEnemy::DisableMaterialFlash() {
	DynamicEnemyMaterial->SetScalarParameterValue(FName("DamageFlashScaler"), 0.f);
}



bool AEnemy::CheckIfAlive() {
	if (HealthPoints > 0) {
		return true;
	}
	else return false;
}

void AEnemy::EnemyDeath() {
	EnemyDeathDelegate.Broadcast(this);
	Destroy();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AEnemy::GetScoreValue() {
	return ScoreValue;
}

float AEnemy::GetSpawnProbability() {
	return SpawnProbability;
}

int AEnemy::GetSpawnCost() {
	return SpawnCost;
}


