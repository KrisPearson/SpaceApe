// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Enemy_AIController.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Pickups/WeaponPickup.h"
#include "BehaviorTree/BehaviorTree.h"


AEnemy::AEnemy()  {

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Mesh")); // this should be skeletal mesh. Alternatively, use the skeletal mesh provided by the CharacterMovementComponent
	EnemyMesh->SetupAttachment(RootComponent);


	bReplicates = true;

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

	// Set the health points the class uses during play to the value of the enemy type's default health points.
	CurrentHealthPoints = HealthPoints;

	//Create a dynamic material in order to enable access to the damage flicker scaler parameter.
	if (BaseMat != nullptr) {
		DynamicEnemyMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
		EnemyMesh->SetMaterial(0, DynamicEnemyMaterial);
	}
}

/*
This is called by the attacking class following a successful attack.
Returns a bool in order to inform the attacking class of the enemy's demise.
*/
void AEnemy::ReceiveDamage(int _DamageAmount, bool& _IsDead, int& _ScoreToAdd) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" ReceiveDamage Called on Server =: %s"), HasAuthority() ? TEXT("True") : TEXT("False")));
	//DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());

	// could pass enemyscore value + damage here? Perhaps add to a single in return value, as opposed to pointer params? Leave it as is for now....

	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING: ServerReceiveDamage Disabled"));

	//	ServerReceiveDamage(_DamageAmount);
	}
	else {
		
		// Add the damage dealt to the score and check whether the damage dealt leaves the enemy at 0hp. If so, then remove the remainder.
		_ScoreToAdd = (CurrentHealthPoints - _DamageAmount) <= 0 ? (_DamageAmount - (CurrentHealthPoints % _DamageAmount)) : _DamageAmount;
		CurrentHealthPoints -= _DamageAmount;

		if (!CheckIfAlive())
		{
			_IsDead = true;
			EnemyDeath();

		} 
		else {
			_IsDead = false;
			MulticastPlayDamageFlash();
		}
	}

}

/*

void AEnemy::ServerReceiveDamage_Implementation(int _DamageAmount) {

	UE_LOG(LogTemp, Warning, TEXT("ServerReceiveDamage_Implementation called"));


	bool B;
	int I;

	ReceiveDamage(_DamageAmount, B, I);
}

bool AEnemy::ServerReceiveDamage_Validate(int _DamageAmount) {
	return true;
}
*/


/*
Tells the material to flicker, following the enemy recieving damage.
Called on server, and runs on server and all clients.
*/
void AEnemy::MulticastPlayDamageFlash_Implementation() {
	if (DynamicEnemyMaterial != nullptr) {
		if (World != nullptr) {
			// Send the current time to the material, which will then handle the duration of the flicker.
			// This approach means that the material's scaler value only needs to be set once, rather than updated by the AEnemy class each frame, or via Timer.
			DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());
		}
		else UE_LOG(LogTemp, Warning, TEXT(" Warning: World = Null"));
	}
	else  UE_LOG(LogTemp, Warning, TEXT(" Warning: DynamicEnemyMaterial = Null"));
}

/*
A check to see whether the enemy has health points remianing.
*/
bool AEnemy::CheckIfAlive() {
	if (CurrentHealthPoints > 0) {
		return true;
	}
	else return false;
}

/*
Destroy this actor following a broadcast to the WaveManager.
*/
void AEnemy::EnemyDeath() {
	SpawnPickup();
	EnemyDeathDelegate.Broadcast(this);
	Destroy();
}


void AEnemy::SpawnPickup() {
	AWeaponPickup* NewPickup = GetWorld()->SpawnActor<AWeaponPickup>(this->GetActorLocation(), this->GetActorRotation());

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

/* DEPRECATED - PART OF OLD APPROACH TO FLICKER
void AEnemy::DisableMaterialFlash() { 
	DynamicEnemyMaterial->SetScalarParameterValue(FName("DamageFlashScaler"), 0.f);
}
*/

//void AEnemy::OnRep_Damaged() { // DEPRECATED
//	PlayDamageFlash();
//}

/* DEPRECATED
void AEnemy::PlayDamageFlash() {
if (DynamicEnemyMaterial != nullptr) {
if (World != nullptr) {
// Send the current time to the material, which will then handle the duration of the flicker.
// This approach means that the material's scaler value only needs to be set once, rather than updated by the AEnemy class each frame, or via Timer.
DynamicEnemyMaterial->SetScalarParameterValue(FName("StartTime"), World->GetTimeSeconds());
}
else UE_LOG(LogTemp, Warning, TEXT(" Warning: World = Null"));
}
else  UE_LOG(LogTemp, Warning, TEXT(" Warning: DynamicEnemyMaterial = Null"));
}
*/

/*
Method for checking whether an enemy's intended move direction is within the playable map bounds
using line plane intersection.
*/
bool AEnemy::CheckIfDirectionIntersects(FVector2D _StartPoint, FVector2D _EndPoint, FVector2D _RectMin, FVector2D _RectMax) {
	// Find min and max X for the segment
	double minX = _StartPoint.X;
	double maxX = _EndPoint.X;

	if (_StartPoint.X > _EndPoint.X) {
		minX = _EndPoint.X;
		maxX = _StartPoint.X;
	}

	// Find the intersection of the segment's and rectangle's x-projections
	if (maxX > _RectMax.X) {
		maxX = _RectMax.X;
	}

	if (minX < _RectMin.X) {
		minX = _RectMin.X;
	}

	// If their projections do not intersect return false
	if (minX > maxX) {
		return false;
	}

	// Find corresponding min and max Y for min and max X we found before
	double minY = _StartPoint.Y;
	double maxY = _EndPoint.Y;

	double dx = _EndPoint.X - _StartPoint.X;

	if (FMath::Abs(dx) > 0.0000001) {
		double a = (_EndPoint.Y - _StartPoint.Y) / dx;
		double b = _StartPoint.Y - a*_StartPoint.X;
		minY = a*minX + b;
		maxY = a*maxX + b;
	}

	if (minY > maxY) {
		double tmp = maxY;
		maxY = minY;
		minY = tmp;
	}

	// Find the intersection of the segment's and rectangle's y-projections
	if (maxY > _RectMax.Y) {
		maxY = _RectMax.Y;
	}

	if (minY < _RectMin.Y) {
		minY = _RectMin.Y;
	}

	// If Y-projections do not intersect return false
	if (minY > maxY) {
		return false;
	}

	return true;
}
