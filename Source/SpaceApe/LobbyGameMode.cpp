// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "SpaceApe.h"
#include "NetworkLobbyPlayerController.h"
#include "Engine/World.h"
#include "NetworkPlayerState.h"

ALobbyGameMode::ALobbyGameMode() {
	/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/SpaceApeCharacter_BP"));


	if (PlayerPawnBPClass.Class != NULL)
	{
	DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/

	// Failsafe
//	PlayerControllerClass = APlayerController_SpaceApe::StaticClass();

	// Blueprinted Version, relies on the asset path obtained from the editor
//	static ConstructorHelpers::FClassFinder<APlayerController_SpaceApe> Controller(TEXT("/Game/Game/PlayerController_SpaceApe_BP"));

//	if (Controller.Class != NULL)
//	{
//		PlayerControllerClass = Controller.Class;
		//UE_LOG(LogTemp, Warning, TEXT("SetController"));
//	}
}


void ALobbyGameMode::BeginPlay() {
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);

}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	ANetworkLobbyPlayerController* JoiningPlayer = Cast<ANetworkLobbyPlayerController>(NewPlayer);

	//if the joining player is a lobby player controller, add them to a list of connected Players
	if (JoiningPlayer)
		ConnectedPlayers.Add(JoiningPlayer);
}

void ALobbyGameMode::Logout(AController* ExitingPlayer) {
	Super::Logout(ExitingPlayer);

	//update the ConnectedPlayers Array and the PlayerList in the lobby whenever a player leaves
	ANetworkLobbyPlayerController* LobbyPlayerController = Cast<ANetworkLobbyPlayerController>(ExitingPlayer);
	if (LobbyPlayerController)
	{
		ConnectedPlayers.Remove(LobbyPlayerController);
		UpdatePlayerList();
	}

}

void ALobbyGameMode::ProdcastChatMessage(const FText & ChatMessage) {
	//call all the connected players and pass in the chat message
	for (ANetworkLobbyPlayerController* Player : ConnectedPlayers)
		Player->Client_ReceiveChatMessage(ChatMessage);
}

void ALobbyGameMode::KickPlayer(int32 PlayerIndex) {
	//call the player to make him destroy his session and leave game
	ConnectedPlayers[PlayerIndex]->Client_GotKicked();
}

void ALobbyGameMode::UpdatePlayerList() {
	//Epmty the PlayerInfo Array to re-populate it
	PlayerInfoArray.Empty();

	//get all the player info from all the connected players
	for (ANetworkLobbyPlayerController* Player : ConnectedPlayers)
	{
		//temporary LobbyPlayerInfo var to hold the player info
		FLobbyPlayerInfo TempLobbyPlayerInfo;

		ANetworkPlayerState* NetworkedPlayerState = Cast<ANetworkPlayerState>(Player->PlayerState);
		if (NetworkedPlayerState)
			TempLobbyPlayerInfo.bPlayerReadyState = NetworkedPlayerState->bIsReady;
		else
			TempLobbyPlayerInfo.bPlayerReadyState = false;

		TempLobbyPlayerInfo.PlayerName = Player->PlayerState->PlayerName;
		PlayerInfoArray.Add(TempLobbyPlayerInfo);
	}

	//call all the players to make them update and pass in the player info array
	for (ANetworkLobbyPlayerController* Player : ConnectedPlayers)
		Player->Client_UpdatePlayerList(PlayerInfoArray);
}

void ALobbyGameMode::StartGameFromLobby() {
	//GetWorld()->ServerTravel(GameMapName);
	UE_LOG(LogTemp, Warning, TEXT("StartGameFromLobby"));

	GetWorld()->ServerTravel(GameMapName);

	//GetWorld()->Exec(GetWorld(), TEXT("servertravel /Game/TwinStickCPP/Maps/SurvivalGame"));
}

bool ALobbyGameMode::IsAllPlayerReady() const {
	for (ANetworkLobbyPlayerController* Player : ConnectedPlayers)
	{
		ANetworkPlayerState* NetworkedPlayerState = Cast<ANetworkPlayerState>(Player->PlayerState);
		if (NetworkedPlayerState)
			if (!NetworkedPlayerState->bIsReady)
				return false;
	}
	return true;
}

void ALobbyGameMode::PlayerRequestUpdate() {
	UpdatePlayerList();
}

	//replaces the current widget on the viewport with a new widget
void ALobbyGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> _NewWidgetClass) {
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




