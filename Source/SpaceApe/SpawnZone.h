// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnZone.generated.h"

UCLASS()
class SPACEAPE_API ASpawnZone : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ASpawnZone();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* SpawnZoneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		float DisplayTime = 1.5f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnF;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnG;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnH;


private:

	TArray<USceneComponent*> SpawnPoints;

	FTimerHandle MeshDisplayTimer;

	bool bIsMeshDisplayed = false;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DisplayMesh();

	void HideMesh();

public:	

	UFUNCTION()
		void RequestDisplayMesh();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetSpawnPointLocation(int _spawnPointIndex);

	
	
};
