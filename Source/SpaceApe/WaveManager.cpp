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

void UWaveManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UWaveManager, WaveTextRenderer);
	DOREPLIFETIME(UWaveManager, WaveTextString);
}


void UWaveManager::WaveManagerInitialisation() {
	if (GetOuter()) {
		World = GEngine->GetWorldFromContextObject(GetOuter());
		UE_LOG(LogTemp, Warning, TEXT("World Assigned"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Outer null"));
	InitialiseWaveText();
	PopulateSpawnLocationsArray(AllSpawnLocationsArray);
	InitialiseEnemyList();

}

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


//bool UWaveManager::GetWorldFromContextObject(UObject * WorldContextObject) {
//	return false;
//}



void UWaveManager::InitialiseEnemyList() {
	UE_LOG(LogTemp, Warning, TEXT("InitialiseEnemyList"));
	
	TArray<AActor*> OutActors;
	TSubclassOf<ASpawnZone> MyClass = ASpawnZone::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetOuter(), MyClass, OutActors);

	UE_LOG(LogTemp, Warning, TEXT("Spawn Zone actors found = %d"),OutActors.Num() );

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


	auto ItemLibrary = UObjectLibrary::CreateLibrary(AEnemy::StaticClass(), true, GIsEditor);
	ItemLibrary->AddToRoot();
	ItemLibrary->LoadBlueprintsFromPath( ENEMYBP_FOLDERLOCATION );
	ItemLibrary->GetObjects<UBlueprintGeneratedClass>(ClassesArray);

	for (int32 i = 0; i < ClassesArray.Num(); ++i) 
	{
		UBlueprintGeneratedClass * BlueprintClass = ClassesArray[i];
		UE_LOG(LogTemp, Warning, TEXT("UBP generated %s"), *BlueprintClass->GetName());

		FString ClassName = *BlueprintClass->GetName();

		if (ClassName.Len() > 4 && ClassName[0] == 'S' && ClassName[1] == 'K' && ClassName[2] == 'E' && ClassName[3] == 'L') {
			UE_LOG(LogTemp, Warning, TEXT("This is a Skeleton Class. No Data created"));
		}
		else {

			AEnemy* NewEnemy = World->SpawnActor<AEnemy>(BlueprintClass, FVector(0, 0, 1000), FRotator(0, 0, 0));
			if (NewEnemy) {
				UE_LOG(LogTemp, Warning, TEXT("Data Actor Spawned"));
				//UE_LOG(LogTemp, Warning, TEXT("Score =  %d"), NewEnemy->GetScoreValue()); //<== works

				FEnemyData* NewEnemyData = new FEnemyData( NewEnemy->GetClass() , NewEnemy->GetSpawnCost(), NewEnemy->GetSpawnProbability());
				NewEnemy->Destroy();

				EnemyDataArray.Add(NewEnemyData);

				if (NewEnemyData->EnemySpawnCost < CheapestEnemyPointCost) {
					CheapestEnemyPointCost = NewEnemyData->EnemySpawnCost;

					UE_LOG(LogTemp, Warning, TEXT(" New CheapestEnemyPointCost =  %d"), NewEnemyData->EnemySpawnCost );
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Classes Array Length =  %d"), ClassesArray.Num());


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
			UE_LOG(LogTemp, Warning, TEXT("New probability =   %f"), EnemyDataArray[i]->SpawnProbability);
			UE_LOG(LogTemp, Warning, TEXT("NormaliseValue =   %f"), NormaliseValue);
		}
	}

}

void UWaveManager::InitialiseWaveText() {

	WaveTextActor = World->SpawnActor<AWaveTextRenderActor>(AWaveTextRenderActor::StaticClass(),
		FVector(0, 0, 250),
		FRotator(90, 0, 180)
		);

	WaveTextActor->SetReplicates(true);

	WaveTextActorRenderer = WaveTextActor->GetTextRender();
}

void UWaveManager::StartSpawning() {
	DisplayWaveIncomingMessage();
	StartActionTimer();
}

void UWaveManager::StartWave() {
	NumberOfGeneratedWaveActions = 0;
	CurrentWaveCount++;
	CurrentWaveActionIndex = 0;

	GenerateWave();
	//DisplayWaveIncomingMessage();
	//StartActionTimer();
	if (World != nullptr)
		World->GetTimerManager().SetTimer(StartWaveTimerHandle, this, &UWaveManager::StartSpawning, 0.2f, false);
	else UE_LOG(LogTemp, Warning, TEXT("WORLD IS NULL"));
}


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


void UWaveManager::PerformWaveAction() {
	UE_LOG(LogTemp, Warning, TEXT("PerformWaveAction."));
	FWaveSpawnAction Action = CurrentWave->WaveActionsList[CurrentWaveActionIndex];

	FVector Location = FindSpawnPoint(Action.SpawnLocationData);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AEnemy* NewEnemy = World->SpawnActor<AEnemy>(Action.EnemyTypeToSpawn,
		Location,
		FRotator(0, 0, 0),
		SpawnParams
		);

	if (NewEnemy) {

		InformTargetZoneOfSpawn(Action.SpawnLocationData.ZoneToSpawnIn);
		
		// DELEGATE <<<<
		NewEnemy->EnemyDeathDelegate.AddDynamic(this, &UWaveManager::RegisterEnemyDeath);
		AliveEnemyArray.Add(NewEnemy);

	}

	CurrentWaveActionIndex++;
	NumberOfGeneratedWaveActions++;

	StartActionTimer();

}

void UWaveManager::RegisterEnemyDeath(AEnemy* _deadEnemyRef) {
	AliveEnemyArray.Remove(_deadEnemyRef);

	if (AliveEnemyArray.Num() <= 0 ) { // <<<<<<<<<<<Should be improved to use NumberOfGeneratedWaveActions (or similar counter)
		StartWave();
	}
}

void UWaveManager::DisplayWaveIncomingMessage() {

	if (WaveTextActor != nullptr) {
		WaveTextActor->DisplayNextWaveMessage(CurrentWaveCount);
	}
}



FSpawnLocation UWaveManager::GenerateSpawnPointData() {

	if (AllSpawnLocationsArray.Num() > 0) {

		int RandomIndex = 0;
		int StrikeValue = 10;

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



FVector UWaveManager::GetSpawnPointFromWaveAction() {

	AActor* TargetZone;

	FVector ReturnVector = FVector(0, 0, 0);

	switch (CurrentWave->WaveActionsList[CurrentWaveActionIndex].SpawnLocationData.ZoneToSpawnIn) {
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
	if (TargetZone != nullptr)
		Cast<ASpawnZone>(TargetZone)->GetSpawnPointLocation((int)CurrentWave->WaveActionsList[CurrentWaveActionIndex].SpawnLocationData.PointInZoneToSpawn);
		//Cast<ASpawnZone>( TargetZone)->GetSpawnPointLocation((int)CurrentWave->WaveActionsList[CurrentWaveActionIndex].PointInZoneToSpawnA) ;
	else {
		UE_LOG(LogTemp, Warning, TEXT("TargetZone undefined."));
	}

	return ReturnVector;
}

FVector UWaveManager::FindSpawnPoint(FSpawnLocation _SpawnLoc) {

	AActor* TargetZone;

	FVector ReturnVector = FVector(0,0,0);

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

	ReturnVector = Cast<ASpawnZone>(TargetZone)->GetSpawnPointLocation((int)_SpawnLoc.PointInZoneToSpawn);

		return ReturnVector;
}

void UWaveManager::GenerateWave() {
	CurrentWave = NULL;

	//make actions
	// add to CurrentWavePattern->WaveActionsList.Add(action)

	int CurrentSpendableEnemyPoints = 1000; // Determines the number and types of enemies. Calculated using current difficulty value.

	//UE_LOG(LogTemp, Warning, TEXT("Initial CurrentSpendableEnemyPoints = %d"), CurrentSpendableEnemyPoints);
	//UE_LOG(LogTemp, Warning, TEXT(" CheapestEnemyPointCost = %d"), CheapestEnemyPointCost);
	TArray<FWaveSpawnAction> SpawnActionsArray;

	while (CurrentSpendableEnemyPoints >= CheapestEnemyPointCost) {
		//UE_LOG(LogTemp, Warning, TEXT("Start of Loop CurrentSpendableEnemyPoints = %d"), CurrentSpendableEnemyPoints);

		// if unable to spend points (too few remaining) then break;


		//FWaveSpawnAction newWaveAction = FWaveSpawnAction(ESpawnPoint::SP_A,  ESpawnZone::SZ_Left, 0.0f);
		FWaveSpawnAction newWaveAction = FWaveSpawnAction(
			GenerateEnemy( CurrentSpendableEnemyPoints ),
			GenerateSpawnPointData(),
			GenerateDelayValue()
		);

		//UE_LOG(LogTemp, Warning, TEXT("Generated Delay Value is = %f"), newWaveAction.DelayUntilSpawn);
		//UE_LOG(LogTemp, Warning, TEXT("CurrentSpendableEnemyPoints = %d"), CurrentSpendableEnemyPoints);

		SpawnActionsArray.Add(newWaveAction);
	}

	CurrentWave = new FWave(SpawnActionsArray);

	UE_LOG(LogTemp, Warning, TEXT("CurrentWave action list length = %d"), CurrentWave->WaveActionsList.Num());

}

float UWaveManager::GenerateDelayValue() {
	//int DelayChance = FMath::RandRange(0, NumberOfGeneratedWaveActions);
	int DelayChance = FMath::RandRange(0, 10);

	//UE_LOG(LogTemp, Warning, TEXT("Delay Chance is = %d"), DelayChance);
	//UE_LOG(LogTemp, Warning, TEXT("Delay Buffer is = %d"), DelayChanceBuffer);
	//UE_LOG(LogTemp, Warning, TEXT("Number of Generated Actions is = %d"), NumberOfGeneratedWaveActions);
	//UE_LOG(LogTemp, Warning, TEXT("ForceDelay is: %s"), ForceDelay ? TEXT("true") : TEXT("false"));
		
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

TSubclassOf<AEnemy> UWaveManager::GenerateEnemy(int& _AvailablePointsToSpend) {

	double p = FMath::FRand();
	double cumulativeProbability = 0.0;
	for (FEnemyData* item : EnemyDataArray) {
		cumulativeProbability += item->SpawnProbability;
		if (p <= cumulativeProbability) {

			if (item->EnemySpawnCost <= _AvailablePointsToSpend) {
				_AvailablePointsToSpend -= item->EnemySpawnCost;
				return item->EnemyClass;
			}
		}
	}

	return  EnemyDataArray[0]->EnemyClass;;
}


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


void UWaveManager::TestSpawn() {

	AEnemy* NewEnemy = World->SpawnActor<AEnemy>(EnemyDataArray[0]->EnemyClass, FVector(0, 0, 1000), FRotator(0, 0, 0));
	if (NewEnemy) {
		UE_LOG(LogTemp, Warning, TEXT("TEST Actor Spawned"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TEST Actor Spawn failed"));
	}
}

