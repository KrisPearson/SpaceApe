// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceApeOnlineGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "SpaceApePawn.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController_SpaceApe.h"
#include "WaveManager.h"
#include "EngineUtils.h"

ASpaceApeOnlineGameMode::ASpaceApeOnlineGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/SpaceApeCharacter_BP"));
	

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	/*

	// Failsafe
	PlayerControllerClass = APlayerController_SpaceApe::StaticClass();

	// Blueprinted Version, relies on the asset path obtained from the editor
	static ConstructorHelpers::FClassFinder<APlayerController_SpaceApe> Controller(TEXT("Blueprint'/Game/TwinStickCPP/Game/PlayerController_SpaceApe_BP.PlayerController_SpaceApe_BP'"));
	
	if (Controller.Class != NULL)
	{
	PlayerControllerClass = Controller.Class;
		UE_LOG(LogTemp, Warning, TEXT("SetController"));
	}
	*/
}




void ASpaceApeOnlineGameMode::BeginPlay() {
	Super::BeginPlay();
	PlayerControllers.Num();
	  


	ChangeMenuWidget(StartingWidgetClass);

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGameMode: BeginPlay Ran"));

	//WaveManager = NewObject<UWaveManager>(this, UWaveManager::StaticClass(), TEXT("WaveManager") );
	//WaveManager->WaveManagerInitialisation();
	//WaveManager->StartWave(); // <<<<<<<<<<<<<<<<< need a timer on this
	//WaveManager->StartWaveManager();

	//GetWorld()->GetTimerManager().SetTimer(StartTimer, WaveManager, &UWaveManager::StartWave, 1.0f, false);
}





void ASpaceApeOnlineGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) {
	Super::SwapPlayerControllers(OldPC, NewPC);
	// Add New Player Controller to Array
	PlayerControllers.Add(NewPC);
}

void ASpaceApeOnlineGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	//AOnlineGamePlayerController* MyPlayerController = Cast<AOnlineGamePlayerController>(Exiting);
	//if (MyPlayerController != nullptr)
	//{
	//	UOnlineGameInstance* MyGameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	//	if (MyGameInstance != nullptr)
	//	{
	//		// Disconnect this player controller 
	//		// From the existing session
	//		MyGameInstance->DestroySessionCaller(MyPlayerController);
	//	}
	//}
}

void ASpaceApeOnlineGameMode::CodeRespawn(APlayerController* _PlayerController, TSubclassOf<APawn> _PlayerPawn) {
	UE_LOG(LogTemp, Warning, TEXT("OnlineGameMode: RespawnPlayerNetwork Ran"));
	// If World Failed for some reason 
	// Don't Proceed
	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	for (TActorIterator<APlayerStart> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Fill in Respawn PlayerStarts Array
		PlayerStarts.Add(*ActorItr);
	}

	// For Respawning : Make sure former player 
	// Gets destroyed if they were
	// Previously alive
	APawn* ControllerPawn = _PlayerController->GetPawn();
	if (ControllerPawn != nullptr)
	{
		ControllerPawn->Destroy();
	}

	// Find Random Spawn Point
	// Respawn There.
	int SpawnIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
	const FTransform SpawnTransform = PlayerStarts[SpawnIndex]->GetActorTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpaceApePawn* SpawnedPawn = World->SpawnActor<ASpaceApePawn>(_PlayerPawn, SpawnTransform, SpawnParams);
	if (SpawnedPawn != nullptr)
	{
		_PlayerController->Possess(SpawnedPawn);
	}
}

void ASpaceApeOnlineGameMode::ServerRespawnPlayerNetwork_Implementation(APlayerController* _PlayerController, TSubclassOf<ACharacter> _PlayerCharacter) {
	if (Role == ROLE_Authority)
	{
		CodeRespawn(_PlayerController, _PlayerCharacter);
	}
	else
	{
		CodeRespawn(_PlayerController, _PlayerCharacter);
	}

}

bool ASpaceApeOnlineGameMode::ServerRespawnPlayerNetwork_Validate(APlayerController* _PlayerController, TSubclassOf<ACharacter> _PlayerCharacter) {
	return true;
}


void ASpaceApeOnlineGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpaceApeOnlineGameMode, PlayerControllers);

	DOREPLIFETIME(ASpaceApeOnlineGameMode, PlayerStarts);



	//DOREPLIFETIME(ASpaceApeOnlineGameMode, 5.0f);

}


//replaces the current widget on the viewport with a new widget
void ASpaceApeOnlineGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> _NewWidgetClass) {
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (_NewWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), _NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}



