// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AEnemy*, EnemyPointer);

class AEnemy;

UCLASS()
class SPACEAPE_API AEnemy : public APawn
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UWorld* World;

public:
	// Sets default values for this pawn's properties
	AEnemy();





	class UMaterialInstanceDynamic* DynamicEnemyMaterial;

	FTimerHandle DamageFlashTimerHandle;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BaseMat;


	UPROPERTY(Category = Scene, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootComp;

	UPROPERTY()
	UFloatingPawnMovement* FloatingMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, Category = Stats, meta = (ClampMin = "10", ClampMax = "1000"))
		int HealthPoints = 10;

	UPROPERTY(EditAnywhere, Category = Stats, meta = (ClampMin = "10", ClampMax = "1000"))
		float MaxSpeed = 500;


		// The Value awarded to the player upon death
	UPROPERTY(EditAnywhere, Category = Stats, meta = (ClampMin = "0", ClampMax = "9999", UIMin = "0", UIMax = "9999"))
		int ScoreValue = 100;

		// The liklihood of this class spawning
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
		float SpawnProbability = 5;

		// Tho cost to spawn the class
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "100", ClampMax = "10000", UIMin = "100", UIMax = "10000"))
		int SpawnCost = 1;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		class UBehaviorTree* BotBehavior;




	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
		virtual bool ReceiveDamage(int _DamageAmount);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReceiveDamage(int _DamageAmount);
	virtual void ServerReceiveDamage_Implementation(int _DamageAmount);
	virtual bool ServerReceiveDamage_Validate(int _DamageAmount);

	
	


	UFUNCTION()
		void EnemyDeath();

	UFUNCTION()
		void DisableMaterialFlash();

	UFUNCTION()
		int GetScoreValue();

	UFUNCTION()
		float GetSpawnProbability();

	UFUNCTION()
		int GetSpawnCost();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnEnemyDeath EnemyDeathDelegate;



protected:

	UPROPERTY(ReplicatedUsing = OnRep_Damaged)
		int CurrentHealthPoints;

	UFUNCTION()
		virtual void OnRep_Damaged();

	UFUNCTION()
		void PlayDamageFlash();


private:




	bool CheckIfAlive();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	
};
