// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "Enemy.h"
//#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "WaveManager.generated.h"


class AEnemy;
class ASpawnZone;


static const FString ENEMYBP_FOLDERLOCATION = "/Game/Blueprints/Enemies";
static const int MAXWAVENUMBER = 10; // the maximum number of enemies in a single given wave

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESpawnPoint : uint8 {
	SP_A 	UMETA(DisplayName = "A"),
	SP_B 	UMETA(DisplayName = "B"),
	SP_C	UMETA(DisplayName = "C"),
	SP_D	UMETA(DisplayName = "D"),
	SP_E	UMETA(DisplayName = "E"),
	SP_F	UMETA(DisplayName = "F"),
	SP_G	UMETA(DisplayName = "G"),
	SP_H	UMETA(DisplayName = "H")
};

UENUM(BlueprintType)
enum class ESpawnZone : uint8 {
	SZ_Left UMETA(DisplayName = "Left Zone"),
	SZ_Right UMETA(DisplayName = "Right Zone"),
	SZ_Top UMETA(DisplayName = "Top Zone"),
	SZ_Bottom UMETA(DisplayName = "Bottom Zone")
};

UENUM(BlueprintType)
enum class EWaveState : uint8 {
	WS_Spawning UMETA(DisplayName = "Spawning"),
	WS_PostSpawn UMETA(DisplayName = "Post Spawn"),
	WS_WaveEnd UMETA(DisplayName = "Wave Ended"),
};


USTRUCT(BlueprintType)
struct FSpawnLocation {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESpawnZone ZoneToSpawnIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESpawnPoint PointInZoneToSpawn;

	FSpawnLocation() {};
	FSpawnLocation(ESpawnZone _ZoneToSpawnIn, ESpawnPoint _PointInZoneToSpawn) {
		ZoneToSpawnIn = _ZoneToSpawnIn;
		PointInZoneToSpawn = _PointInZoneToSpawn;
	}
};

USTRUCT(BlueprintType)
struct  FWaveSpawnAction {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> EnemyTypeToSpawn; // DELETE ME?
	FSpawnLocation SpawnLocationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayUntilSpawn;

	FWaveSpawnAction() {};

	
	FWaveSpawnAction(TSubclassOf<AEnemy> _EnemyTypeToSpawn, FSpawnLocation _SpawnLocationData, float _DelayUntilSpawn) {
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		EnemyTypeToSpawn = _EnemyTypeToSpawn;
		SpawnLocationData = _SpawnLocationData,
		DelayUntilSpawn = _DelayUntilSpawn;
	}
	
};


USTRUCT(BlueprintType)
struct  FWave {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FWaveSpawnAction> WaveActionsList;

	FWave() {};

	FWave(TArray<FWaveSpawnAction> _WaveActionsList) {
		WaveActionsList = _WaveActionsList;
	}
};


USTRUCT(BlueprintType)
struct FEnemyData {
	GENERATED_BODY()

		TSubclassOf<AEnemy>  EnemyClass;
	int EnemySpawnCost;
	float SpawnProbability;

	FEnemyData() {};
	FEnemyData(TSubclassOf<AEnemy> _EnemyClass, int _EnemySpawnCost, float _SpawnProbability) {
		EnemyClass = _EnemyClass;
		EnemySpawnCost = _EnemySpawnCost;
		SpawnProbability = _SpawnProbability;
	};
};

/**
 * 
 */
UCLASS(BlueprintType/*, meta = (/*BlueprintSpawnableComponent*/)
class  UWaveManager : public UObject
{
	GENERATED_BODY()

	UWaveManager();

	// UObjects don't have a reference to World by default, so this is assigned in 
	// WaveManagerInitialisation by getting a reference from the owning class.
	UWorld* World;

	UPROPERTY()
	TArray<UBlueprintGeneratedClass *> ClassesArray;

	TArray<FEnemyData*> EnemyDataArray;

	FTimerHandle SpawnLoopTimerHandle;
	FTimerHandle StartWaveTimerHandle;

	int CurrentWaveActionIndex = 0;

	int CurrentWaveCount;

	int WaveTextStringIndex = 0;



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* LeftSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* RightSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* BottomSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* TopSpawnZone;


private:
	// Keeps track of all remaining enemies. 
	TArray<AEnemy*> AliveEnemyArray; // Could this approach be improved somewhat?

	// The current wave to be spawned. Assigned on wave generation.
	FWave* CurrentWave;

	// Populated during initialisation, Spawn Locations in this array are used to identify the spawn vectors of enemies.
	TArray<FSpawnLocation> AllSpawnLocationsArray;

	// Used during wave generation to prevent multiple enemies spawning atop one another.
	TArray<int> SpawnPointsInUse; // POSSIBLE BUG: Is this ever reset? Needs investogating

	int NumberOfGeneratedWaveActions; // This variable is not currently in use. Possible Intended uses include: Limiting enemy numbers, Affecting delay values,

	// Dictates whether a wave action will be forced to delay spawning
	bool ForceDelay = false;

	// Used when generating delay. If it reaches zero, then a delay will be forced. 
	int DelayChanceBuffer;

	// The spawn zone the wave generator will attempt to spawn in when making a wave action. 
	// This will be set at random to another zone following failed attempts. 
	ESpawnZone CurrentSpawnZone;

	// Assigned when generating EnemyData; this will be used when generating waves to check whether there are enough points remaining to spawn the cheapest enemy.
	int CheapestEnemyPointCost = 1000;

	class AWaveTextRenderActor* WaveTextActor;
	class UTextRenderComponent* WaveTextActorRenderer;

	bool bHasFinishedSpawning;

#pragma region Initialisation Functions

	UFUNCTION(BlueprintCallable)
	void WaveManagerInitialisation();

	void LocateSpawnZones();

	void InitialiseEnemyList();

	void InitialiseWaveText();

	void PopulateSpawnLocationsArray(TArray<FSpawnLocation>& _ArrayToPopulate);

#pragma endregion These methods are used to initialise variables and arrays for the class. WaveManagerInitialisation should be called by the owning class (GameMode) following instantiation.


#pragma region Wave Generating Functions
	UFUNCTION()
		void GenerateWave();


	UFUNCTION()
		TSubclassOf<AEnemy> GenerateEnemy(int& _AvailablePointsToSpend);

	UFUNCTION()
		FSpawnLocation GenerateSpawnPointData();


	UFUNCTION()
		float GenerateDelayValue();
#pragma endregion These methods are associated with generating spawn data for a  wave.

#pragma region Wave Loop Functions

	UFUNCTION(BlueprintCallable)
		void StartWave();

	UFUNCTION()
		void DisplayWaveIncomingMessage();

	void StartWaveTimer();

	UFUNCTION()
		void StartActionTimer();

	UFUNCTION()
		void StartSpawning();

	UFUNCTION()
		void PerformWaveAction();

	FVector FindSpawnPoint(FSpawnLocation _SpawnLoc);


	void InformTargetZoneOfSpawn(ESpawnZone _TargetZone);

	UFUNCTION()
		void RegisterEnemyDeath(AEnemy* _deadEnemyRef);

#pragma endregion These methods are either involved with the spawning of or death of enemies, and the handling of wave information.


};
