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
UCLASS(meta = (BlueprintSpawnableComponent))
class  UWaveManager : public UObject
{
	GENERATED_BODY()

	UWaveManager();


	UWorld* World;

	UPROPERTY()
	TArray<UBlueprintGeneratedClass *> ClassesArray;

	TArray<FEnemyData*> EnemyDataArray;

	FTimerHandle SpawnLoopTimerHandle;
	FTimerHandle WaveTextTimerHandle;

	int CurrentWaveActionIndex = 0;

	int CurrentWaveCount;

	int WaveTextStringIndex = 0;
	FString WaveTextString;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* LeftSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* RightSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* BottomSpawnZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AActor* TopSpawnZone;

	UFUNCTION(BlueprintCallable)
		void WaveManagerInitialisation();

private:


	TArray<AEnemy*> AliveEnemyArray;

	FWave* CurrentWave;

	TArray<FSpawnLocation> AllSpawnLocationsArray;
	TArray<int> SpawnPointsInUse;
	bool ForceDelay = false;

	int NumberOfGeneratedWaveActions;
	int DelayChanceBuffer;

	ESpawnZone CurrentSpawnZone;

	int CheapestEnemyPointCost = 1000;

	void PopulateSpawnLocationsArray(TArray<FSpawnLocation>& _ArrayToPopulate);

	class UTextRenderComponent* WaveTextRenderer;

	UFUNCTION(BlueprintCallable, Category = Misc, meta = (WorldContext = WorldContextObject))
		static bool GetWorldFromContextObject(UObject * WorldContextObject);




	UFUNCTION(BlueprintCallable)
		void InitialiseEnemyList();

		void InitialiseWaveText();

	//UFUNCTION()
	//	bool PerformWaveAction(struct FWaveSpawnAction& _WaveAction);

	void WaveTextTimerFunc();

	UFUNCTION()
		FSpawnLocation GenerateSpawnPointData();

	UFUNCTION()
		void PerformWaveAction();

	UFUNCTION()
		void RegisterEnemyDeath(AEnemy* _deadEnemyRef);

	UFUNCTION()
		void DisplayWaveIncomingMessage();

	UFUNCTION()
		void GenerateWave();

	UFUNCTION()
		FVector GetSpawnPointFromWaveAction();

	FVector FindSpawnPoint(FSpawnLocation _SpawnLoc);

	UFUNCTION()
		float GenerateDelayValue();

	UFUNCTION()
		TSubclassOf<AEnemy> GenerateEnemy(int& _AvailablePointsToSpend);

	UFUNCTION()
		void StartWave();

	UFUNCTION()
		void StartActionTimer();


	void InformTargetZoneOfSpawn(ESpawnZone _TargetZone);






	UFUNCTION(BlueprintCallable) // DELETE ME AT A LATER DATE
	void TestSpawn();


};
