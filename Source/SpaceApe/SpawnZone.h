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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:

	TArray<USceneComponent*> SpawnPoints;

	FTimerHandle MeshDisplayTimer;


	UPROPERTY(ReplicatedUsing = OnRep_SetMeshVisibility)
	bool bIsMeshDisplayed = false;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DisplayMesh();

	void HideMesh();

	UFUNCTION()
	void OnRep_SetMeshVisibility();


/*
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDisplayMesh();
		void MulticastDisplayMesh_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastHideMesh();
		void MulticastHideMesh_Implementation();

*/

public:	

	UFUNCTION()
		void RequestDisplayMesh();

	FVector GetSpawnPointLocation(int _spawnPointIndex);

	
	
};
