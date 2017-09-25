// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Enemy_AIController.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"




// Sets default values
AEnemy::AEnemy()  {

	//RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = RootComp;


	//CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("Character Movement Component"));
	//CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
	//FloatingMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement Component"));

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh"));
	EnemyMesh->SetupAttachment(RootComponent);

	//FloatingMovementComponent->MaxSpeed = MaxSpeed;

	bReplicates = true;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.



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

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, CurrentHealthPoints);
}

void AEnemy::BeginPlay() {
		Super::BeginPlay();

		World = GetWorld();

		CurrentHealthPoints = HealthPoints;

		if (BaseMat != nullptr) {
			DynamicEnemyMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
			EnemyMesh->SetMaterial(0, DynamicEnemyMaterial);			

		}


}

bool AEnemy::ReceiveDamage(int _DamageAmount) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" ReceiveDamage Called on Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));
	//DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());

	if (Role < ROLE_Authority)
	{
		ServerReceiveDamage(_DamageAmount);
	}
	else {
		CurrentHealthPoints -= _DamageAmount;
		if (!CheckIfAlive())
		{
			EnemyDeath();
			return true;
		}
		else PlayDamageFlash();

	}

	return false;
}

void AEnemy::ServerReceiveDamage_Implementation(int _DamageAmount) {
	ReceiveDamage(_DamageAmount);
}

bool AEnemy::ServerReceiveDamage_Validate(int _DamageAmount) {
	return true;
}

void AEnemy::OnRep_Damaged() {
	PlayDamageFlash();
}


void AEnemy::PlayDamageFlash() {
	if (DynamicEnemyMaterial != nullptr) {
		if (World != nullptr) {
			DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());
		}
		else UE_LOG(LogTemp, Warning, TEXT(" Warning: World = Null"));
	}
	else  UE_LOG(LogTemp, Warning, TEXT(" Warning: DynamicEnemyMaterial = Null"));
}



//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" PlayDamageFlash_Implementation Called on Server =: %s"), Role == ROLE_Authority ? TEXT("True") : TEXT("False")));





/*
void AEnemy::DamageFlash() {



	//DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());

	//DynamicEnemyMaterial->SetScalarParameterValue(FName("DamageFlashScaler"), 0.5f); // ask material to flicker here
}

void AEnemy::DamageFlash_Implementation() {
	DamageFlash();
}

bool AEnemy::DamageFlash_Validate() {
	return true;
}
*/




void AEnemy::DisableMaterialFlash() {
	DynamicEnemyMaterial->SetScalarParameterValue(FName("DamageFlashScaler"), 0.f);
}


bool AEnemy::CheckIfAlive() {
	if (CurrentHealthPoints > 0) {
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


