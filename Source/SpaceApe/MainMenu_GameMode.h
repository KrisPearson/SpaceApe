// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEAPE_API AMainMenu_GameMode : public AGameMode
{
	GENERATED_BODY()
private:

public:
	AMainMenu_GameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++ Game Info")
		TArray<APlayerController*> PlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++ Game Info")
		TArray<class APlayerStart*> PlayerStarts;

	void CodeRespawn(APlayerController* _PlayerController, TSubclassOf<APawn> _PlayerCharacter);

	/**
	* used to swap a viewport/connection's PlayerControllers when seamless traveling and the new GameMode's
	* controller class is different than the previous
	* includes network handling
	* @param OldPC - the old PC that should be discarded
	* @param NewPC - the new PC that should be used for the player
	*/
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

	/** Called when a Controller with a PlayerState leaves the match. */
	virtual void Logout(AController* Exiting) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintNativeEvent, Category = "C++ Game Info")
	//	void RespawnPlayer(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);
	//virtual void RespawnPlayer_Implementation(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void ServerRespawnPlayerNetwork(APlayerController* _PlayerController, TSubclassOf<class ACharacter> _PlayerCharacter);
	virtual void ServerRespawnPlayerNetwork_Implementation(APlayerController* _PlayerController, TSubclassOf<class ACharacter> _PlayerCharacter);
	virtual bool ServerRespawnPlayerNetwork_Validate(APlayerController* _PlayerController, TSubclassOf<class ACharacter> _PlayerCharacter);



	FORCEINLINE int GetPCInGame() const { return PlayerControllers.Num(); }

	UFUNCTION(BlueprintCallable, Category = "UMG")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> _NewWidgetClass);

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
		UUserWidget* CurrentWidget;

};

