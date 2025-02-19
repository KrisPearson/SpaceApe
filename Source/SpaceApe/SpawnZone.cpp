// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnZone.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"


// Sets default values
ASpawnZone::ASpawnZone()
{

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	SpawnZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnZoneMesh"));
	SpawnZoneMesh->SetupAttachment(RootComponent);

	SpawnZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnA = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point A"));
	SpawnB = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point B"));
	SpawnC = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point C"));
	SpawnD = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point D"));
	SpawnE = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point E"));
	SpawnF = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point F"));
	SpawnG = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point G"));
	SpawnH = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point H"));

	SpawnPoints.Add(SpawnA);
	SpawnPoints.Add(SpawnB);
	SpawnPoints.Add(SpawnC);
	SpawnPoints.Add(SpawnD);
	SpawnPoints.Add(SpawnE);
	SpawnPoints.Add(SpawnF);
	SpawnPoints.Add(SpawnG);
	SpawnPoints.Add(SpawnH);

	// Offset the spawn points, placing them behind the actor in a row.
	FVector SpawnPointOffset = FVector(-900, -400, 0);
	for (int i = 0; i < SpawnPoints.Num(); i++) {
		SpawnPoints[i]->SetWorldLocation(SpawnPointOffset);
		SpawnPointOffset = FVector(SpawnPointOffset.X -225, SpawnPointOffset.Y, SpawnPointOffset.Z);
	}

}

void ASpawnZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnZone, bIsMeshDisplayed);

}

// Called when the game starts or when spawned
void ASpawnZone::BeginPlay()
{
	Super::BeginPlay();
	SpawnZoneMesh->SetVisibility(0);
	bIsMeshDisplayed = false;
}

void ASpawnZone::RequestDisplayMesh() {
	DisplayMesh();
}

void ASpawnZone::DisplayMesh() {
	bIsMeshDisplayed = true;
	GetWorld()->GetTimerManager().SetTimer(MeshDisplayTimer, this, &ASpawnZone::HideMesh, DisplayTime, false);
	SpawnZoneMesh->SetVisibility(1);
}

void ASpawnZone::HideMesh() {
	bIsMeshDisplayed = false;
	SpawnZoneMesh->SetVisibility(0);
}

void ASpawnZone::OnRep_SetMeshVisibility() {
	SpawnZoneMesh->SetVisibility(bIsMeshDisplayed);
}

/*
Returns the cartesian 
*/
FVector ASpawnZone::GetSpawnPointLocation(int _spawnPointIndex) {
	return SpawnPoints[_spawnPointIndex]->GetComponentLocation();
}

