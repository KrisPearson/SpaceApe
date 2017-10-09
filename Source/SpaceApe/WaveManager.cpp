// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveManager.h"
#include "SpaceApe.h"
#include "SpawnZone.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/AssetRegistry/Public/ARFilter.h"
//#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "GameModeClasses/WaveTextRenderActor.h"
#include "EngineUtils.h"
#include "Enemy.h"

UWaveManager::UWaveManager() {

}

/*
Calls several initialisation functions which are required to set up various variables and references for this
class to function. It should be called by the owning class, which is intended to be a Game Mode. Following this,
the owning actor should call StartWave once, which will initiate the WaveManagers spawning loop.
*/
void UWaveManager::WaveManagerInitialisation() {
	// First of all, we need a reference to World. UObjects don't have one by default, so we use the owner in order to find one.
	if (GetOuter()) {
		World = GEngine->GetWorldFromContextObject(GetOuter());
		UE_LOG(LogTemp, Warning, TEXT("World Assigned"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Outer null"));
	InitialiseWaveText();
	LocateSpawnZones();
	PopulateSpawnLocationsArray(AllSpawnLocationsArray);
	InitialiseEnemyList();
}


/*
Fills an array of FSpawnLocation  structs with all of the locations in each of the spawn zones.
This is currently predifined and hardcoded to provide the array with 32 (4*8) spawn locations, based upon the eight locations in each of the four spawn zones.
Must be called in order for newly created FWaveSpawnAction to be assigned during the wave generation process.
*/
void UWaveManager::PopulateSpawnLocationsArray(TArray<FSpawnLocation>& _ArrayToPopulate) {

	for (int z = 0; z < 4; z++) {
		for (int p = 0; p < 8; p++) {

			ESpawnZone zone = StaticCast<ESpawnZone>(z);
			ESpawnPoint point = StaticCast<ESpawnPoint>(p);

			FSpawnLocation NewLoc = FSpawnLocation(
				zone,
				point
			);

			_ArrayToPopulate.Add(NewLoc);		
		}
	}
}


/*
This 'Hardcoded' method uses predefined strings to locate specific actors in the world by name, before
assigning variables referencing them. These are used to locate spawn locations, as well as to provide
visual feedback to the player.
*/
void UWaveManager::LocateSpawnZones() {
	UE_LOG(LogTemp, Warning, TEXT("LocateSpawnZones"));

	TArray<AActor*> OutActors;
	TSubclassOf<ASpawnZone> MyClass = ASpawnZone::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetOuter(), MyClass, OutActors);

	UE_LOG(LogTemp, Warning, TEXT("Spawn Zone actors found = %d"), OutActors.Num());

	for (int i = 0; i < OutActors.Num(); i++) {
		if (OutActors[i]->GetName() == "SpawnZone_Bottom") {
			BottomSpawnZone = OutActors[i];
		}
		else if (OutActors[i]->GetName() == "SpawnZone_Top") {
			TopSpawnZone = OutActors[i];
		}
		else if (OutActors[i]->GetName() == "SpawnZone_Left") {
			LeftSpawnZone = OutActors[i];
		}
		else if (OutActors[i]->GetName() == "SpawnZone_Right") {
			RightSpawnZone = OutActors[i];
		}
		else UE_LOG(LogTemp, Warning, TEXT("Error: No Zones found "));
	}
}

/*
Loads all of the blueprints located in ENEMYBP_FOLDERLOCATION and  generates an FEnemyData for each Enemy BP,
copying the relevant information for spawning into the struct before adding it to the EnemyDataArray.
The cheapest enemy to spawn is recorded for use in the spawning loop when generating a wave.
Finally, using each of the FEnemyData, the probability to spawn is calculated by dividing the spawn probability 
of each enemy by the sum of all enemy spawn probabilities. 
*/
void UWaveManager::InitialiseEnemyList() {
	UE_LOG(LogTemp, Warning, TEXT("InitialiseEnemyList"));
	
	// Use UObjectLibrary to load classes derived from AEnemy which are located in the ENEMYBP_FOLDERLOCATION. 
	auto ItemLibrary = UObjectLibrary::CreateLibrary(AEnemy::StaticClass(), true, GIsEditor);
	ItemLibrary->AddToRoot();
	ItemLibrary->LoadBlueprintsFromPath( ENEMYBP_FOLDERLOCATION );
	ItemLibrary->GetObjects<UBlueprintGeneratedClass>(ClassesArray);

	//Iterate through each found class, processing it as required.
	for (int32 i = 0; i < ClassesArray.Num(); ++i) 
	{
		UBlueprintGeneratedClass * BlueprintClass = ClassesArray[i];
		UE_LOG(LogTemp, Warning, TEXT("UBP generated %s"), *BlueprintClass->GetName());

		// Ignore skeleton classes, by ignoring any identified classes with the prefix "SKEL" in the Class Name.
		FString ClassName = *BlueprintClass->GetName();
		if (ClassName.Len() > 4 && ClassName[0] == 'S' && ClassName[1] == 'K' && ClassName[2] == 'E' && ClassName[3] == 'L') {
			UE_LOG(LogTemp, Warning, TEXT("This is a Skeleton Class. No Data created"));
		}
		else {
			// Process a class by spawning an instance, which allows us to get the Class Type of the instance, whilst also getting
			// other variables needed to generate FEnemyData. 
			AEnemy* NewEnemy = World->SpawnActor<AEnemy>(BlueprintClass, FVector(0, 0, -1000), FRotator(0, 0, 0));
			if (NewEnemy) {
				UE_LOG(LogTemp, Warning, TEXT("Data Actor Spawned"));

				FEnemyData* NewEnemyData = new FEnemyData( NewEnemy->GetClass() , NewEnemy->GetSpawnCost(), NewEnemy->GetSpawnProbability());
				NewEnemy->Destroy(); // Destroy the instance (it would be possible to set up object pooling here instead, if desired.)

				// Add the new data entry to the array.
				EnemyDataArray.Add(NewEnemyData);

				// Compare the cost to spawn the enemy from teh new data entry with the current cheapest enemy.
				// This is used during wave generation to check if a wave has enough points to spawn the cheapest enemy.
				if (NewEnemyData->EnemySpawnCost < CheapestEnemyPointCost) {
					CheapestEnemyPointCost = NewEnemyData->EnemySpawnCost;
					UE_LOG(LogTemp, Warning, TEXT(" New CheapestEnemyPointCost =  %d"), NewEnemyData->EnemySpawnCost );
				}
			}
		}
	}

		// Normalise spawn probabilities for each data set, so that the sum of all probability equals 1
	if ( EnemyDataArray.Num() > 0 ) {

		float NormaliseValue =   0;
			
			// Calculate the value used to normalise the probabilities, by dividing it by the sum of all probabilty values
		for (int32 i = 0; i < EnemyDataArray.Num(); ++i) {
			NormaliseValue += EnemyDataArray[i]->SpawnProbability;
		}

			// Amend each probability value
		for (int32 i = 0; i < EnemyDataArray.Num(); ++i) {
			EnemyDataArray[i]->SpawnProbability = EnemyDataArray[i]->SpawnProbability / NormaliseValue;
		}
	}
}


/*
Spawns a custom text render actor in the background, which is used to display messages to the player(s) during runtime. 
*/
void UWaveManager::InitialiseWaveText() {

	WaveTextActor = World->SpawnActor<AWaveTextRenderActor>(AWaveTextRenderActor::StaticClass(),
		FVector(0, 0, 250),
		FRotator(90, 0, 180)
		);

	WaveTextActor->SetReplicates(true);

	WaveTextActorRenderer = WaveTextActor->GetTextRender();
}



/*
Resets the values associated with recording the progress of each wave, increases the wave count
and calls GenerateWave. It then triggers a timer handle to call the Start Spawning method.
*/
void UWaveManager::StartWave() {
	NumberOfGeneratedWaveActions = 0;
	CurrentWaveActionIndex = 0;
	CurrentWaveCount++;

	GenerateWave();
	//DisplayWaveIncomingMessage();
	//StartActionTimer();
	if (World != nullptr)
		World->GetTimerManager().SetTimer(StartWaveTimerHandle, this, &UWaveManager::StartSpawning, 0.2f, false);
	else UE_LOG(LogTemp, Warning, TEXT("WORLD IS NULL"));
}

/*
Calls the StartActionTimer method, whilst also calling the DisplayWaveIncomingMessage. 
This method is intended to be triggered by the conclusion of a Timer Handle; adding a delay before the wave starts.
*/
void UWaveManager::StartSpawning() {
	DisplayWaveIncomingMessage();
	StartActionTimer();
}

/*
Calls PerformWaveAction, either directly or via a timer depending on whether the wave action has a delay value greater than zero.
This and PerformWaveAction make up the spawning cycle, and will loop through eachother until the CurrentWaveActionIndex no longer
less than the CurrentWaves action count.
*/
void UWaveManager::StartActionTimer() {

	if ((CurrentWaveActionIndex )< CurrentWave->WaveActionsList.Num()) {

		float Delay = CurrentWave->WaveActionsList[CurrentWaveActionIndex].DelayUntilSpawn;
		if (Delay > 0) {
			World->GetTimerManager().SetTimer(SpawnLoopTimerHandle, this, &UWaveManager::PerformWaveAction, Delay, false);
		}
		else {

			PerformWaveAction();
		}
	}
}

/*
Using the FWaveSpawnAction data supplied by the WaveActionList, this method spawns an enemy of
the defined type at the required location.
*/
void UWaveManager::PerformWaveAction() {
	UE_LOG(LogTemp, Warning, TEXT("PerformWaveAction."));
	FWaveSpawnAction Action = CurrentWave->WaveActionsList[CurrentWaveActionIndex];

	FVector Location = FindSpawnPoint(Action.SpawnLocationData);

	// Setup the spawn parameters so that the Enemy will always spawn, regardless of colissions. 
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AEnemy* NewEnemy = World->SpawnActor<AEnemy>(Action.EnemyTypeToSpawn,
		Location,
		FRotator(0, 0, 0), // this could be set to face inwards, perhaps using the spawn zone's rotation
		SpawnParams
		);

	if (NewEnemy) {
		// Tell the spawning zone that an enemy is spawning, so that the zone can provide  visual feeback to the player(s).
		InformTargetZoneOfSpawn(Action.SpawnLocationData.ZoneToSpawnIn);
		
		// Assign a delegate, so that the Wave Manager is informed when an enemy dies. 
		NewEnemy->EnemyDeathDelegate.AddDynamic(this, &UWaveManager::RegisterEnemyDeath);
		AliveEnemyArray.Add(NewEnemy); // Add the enemy to an array
		// This could potentially be improved, as currently, if  bug or other unforeseen event causes the enemy to be unkillable by
		// the player, or this delegate fails to inform the manager of its death, then no new waves will spawn.

	}

	CurrentWaveActionIndex++; // Increased in order to spawn the next wave action.
	NumberOfGeneratedWaveActions++; // <<<<<<< Deprecated? Consider the best/safest way to keep track of the current number of enemies spawned

	StartActionTimer();

}


/*
This is called by the delegate bound to the spawned enemies. It is vital for keeping track of the remaining number of enemies, 
and ultimately starting the next wave,
*/
void UWaveManager::RegisterEnemyDeath(AEnemy* _deadEnemyRef) {
	AliveEnemyArray.Remove(_deadEnemyRef);

	if (AliveEnemyArray.Num() <= 0 ) { // <<<<<<<<<<<Should be improved to use NumberOfGeneratedWaveActions (or similar counter)
		StartWave();
	}
}

/*
Asks the WaveTextActor to display the current wave number
*/
void UWaveManager::DisplayWaveIncomingMessage() {
	if (WaveTextActor != nullptr) {
		WaveTextActor->DisplayNextWaveMessage(CurrentWaveCount);
	}
}

/*
This Generates a spawn location for the wave action. It attempts to find a lcation
from the Spawn Locations Arraythat an enemy is not already spawning at
and it may force a delay if it fails to do so.
*/
FSpawnLocation UWaveManager::GenerateSpawnPointData() {

	if (AllSpawnLocationsArray.Num() > 0) {

		int RandomIndex = 0;
		int StrikeValue = 10; // the number of times the method is permited to fail to find a location before it forces a delay

		// these values are used to identify the various spawn locations, and are ultimately used to get a location from the Locations Array.
		int MinVal = 0;
		int MaxVal = 0;

		switch (CurrentSpawnZone) {
		case ESpawnZone::SZ_Left:
			MinVal = 0;
			MaxVal = 7;
			break;
		case ESpawnZone::SZ_Right:
			MinVal = 8;
			MaxVal = 15;
			break;
		case ESpawnZone::SZ_Top:
			MinVal = 16;
			MaxVal = 23;
			break;
		case ESpawnZone::SZ_Bottom:
			MinVal = 24;
			MaxVal = 31;
			break;
		}

		bool CanContinue = false;
		while (!CanContinue) {

			RandomIndex = FMath::RandRange(MinVal, MaxVal);

			if (!SpawnPointsInUse.Find(RandomIndex)) {
				CanContinue = true;
			}
			else {
				StrikeValue--;
				if (StrikeValue <= 0) {

					if (FMath::FRand() > 0.5f) {
						ForceDelay = true;
						CanContinue = true;
					}

					else {
						CurrentSpawnZone = StaticCast<ESpawnZone>(FMath::RandRange(0, 3));
						CanContinue = true;
					}
				}


			}
		}
		SpawnPointsInUse.Add(RandomIndex);
		return AllSpawnLocationsArray[RandomIndex];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Unable To Generate Spawn Point Data. Array Empty. Returning Default Value."));
		return FSpawnLocation(ESpawnZone::SZ_Left, ESpawnPoint::SP_A);
	}
}


/*
Returns the cartesian coodrinates at which the object will spawn by reading the information
in the SpawnActions SpawnLocation variable.
*/
FVector UWaveManager::FindSpawnPoint(FSpawnLocation _SpawnLoc) {

	AActor* TargetZone;

	FVector ReturnVector = FVector(0, 0, 0);

	switch (_SpawnLoc.ZoneToSpawnIn) {
	case ESpawnZone::SZ_Bottom:
		TargetZone = BottomSpawnZone;
		break;
	case ESpawnZone::SZ_Left:
		TargetZone = LeftSpawnZone;
		break;
	case ESpawnZone::SZ_Right:
		TargetZone = RightSpawnZone;
		break;
	case ESpawnZone::SZ_Top:
		TargetZone = TopSpawnZone;
		break;
	default:
		return ReturnVector;
		break;
	}
	if (TargetZone != nullptr) {
		ReturnVector = Cast<ASpawnZone>(TargetZone)->GetSpawnPointLocation((int)_SpawnLoc.PointInZoneToSpawn);
	}
	return ReturnVector;

}

/*
Generates FWave for the next wave, filling its Spawn Action array by spending points on enemies, whilst 
assigning their spawn location and delay before eaxh action. The while loop generating the spawn data will
continue until the number of points left to spend falls below the cost of the cheapest enemy.
*/
void UWaveManager::GenerateWave() {
	CurrentWave = NULL;

	// Determines the number and types of enemies. (Eventually this will be recalculated each wave in order to increase difficulty as the game progresses)
	int CurrentSpendableEnemyPoints = 1000; 

	TArray<FWaveSpawnAction> SpawnActionsArray;

	// loop though, spending the remaining spendable points until not enough remain to pay for the cheapest enemy.
	while (CurrentSpendableEnemyPoints >= CheapestEnemyPointCost) {

		FWaveSpawnAction newWaveAction = FWaveSpawnAction(
			GenerateEnemy( CurrentSpendableEnemyPoints ),
			GenerateSpawnPointData(),
			GenerateDelayValue()
		);

		SpawnActionsArray.Add(newWaveAction);
	}

	CurrentWave = new FWave(SpawnActionsArray);

}

float UWaveManager::GenerateDelayValue() {
	//int DelayChance = FMath::RandRange(0, NumberOfGeneratedWaveActions);
	int DelayChance = FMath::RandRange(0, 10);
		
		// No delay
	if (DelayChance <= 3 && !ForceDelay) return 0;

		// Check buffer
	else if (DelayChanceBuffer <= 0 || ForceDelay) {

		DelayChanceBuffer = DelayChance;
		ForceDelay = false;

		float DelayValue = FMath::RandRange(1, 3);
		//UE_LOG(LogTemp, Log, TEXT("Delay Forced! Value = %f"), DelayValue);
		return DelayValue;
	}
		// Reduce buffer
	else {
		DelayChanceBuffer--;
		return 0.0f;
	}
}

/*
Gets a random enemy based on a random probability value and the number of points available to spend using normal distribution.
By passing in the current remaining points as a reference, this method reduces the value itself, allowing
the method to be used as a single line in the FWaveAction constructor parameter.
*/
TSubclassOf<AEnemy> UWaveManager::GenerateEnemy(int& _AvailablePointsToSpend) {


	int points = _AvailablePointsToSpend;

	double p = FMath::FRand(); // Generate a random number between 0 and 1.
	double cumulativeProbability = 0.0;
	for (FEnemyData* enemyData : EnemyDataArray) { // cucle through all of the enemy data
		cumulativeProbability += enemyData->SpawnProbability; // add the spawn probability of the current enemyData to the cummulative probability.
		if (p <= cumulativeProbability) {

			// check whether there is enough points remaining to afford this enemy
			if (enemyData->EnemySpawnCost <= _AvailablePointsToSpend) {
				_AvailablePointsToSpend -= enemyData->EnemySpawnCost; // reduce the Current
				return enemyData->EnemyClass;
			}
		}
	}

	return  EnemyDataArray[0]->EnemyClass;;
}

/*
Tells the region in which an enemy is spawning to provide visual feedback to the player(s).
The Zone informed is currently dictated by the four predifined spawn zones identified during initialisation.
*/
void UWaveManager::InformTargetZoneOfSpawn(ESpawnZone _TargetZone) {
	switch (_TargetZone) {
	case ESpawnZone::SZ_Left:
		Cast<ASpawnZone>(LeftSpawnZone)->RequestDisplayMesh();
		break;
	case ESpawnZone::SZ_Right:
		Cast<ASpawnZone>(RightSpawnZone)->RequestDisplayMesh();
		break;
	case ESpawnZone::SZ_Top:
		Cast<ASpawnZone>(TopSpawnZone)->RequestDisplayMesh();
		break;
	case ESpawnZone::SZ_Bottom:
		Cast<ASpawnZone>(BottomSpawnZone)->RequestDisplayMesh();
		break;
	}
}

