// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceApePlayerCharacter.generated.h"

UCLASS()
class SPACEAPE_API ASpaceApePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(Category = Scene, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootComp;

	UPROPERTY(Category = Scene, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* TiltComponent;

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	//	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	//	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//	class USpringArmComponent* CameraBoom;

public:
	ASpaceApePlayerCharacter();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

	/* The max tilt angle */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MaxAngle;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	void HandleMovement(float deltaSeconds);

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;


private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"), Replicated)
	int CurrentScore;

	UWorld* World;


protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float PlayerProjectileDamage = 10.f;

	UFUNCTION()
	void DealDamage(AActor* _Enemy);


public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	//	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	UFUNCTION(Reliable, Server, WithValidation)
		void ServerFire(FVector _FireDirection);
	void ServerFire_Implementation(FVector _FireDirection);
	bool ServerFire_Validate(FVector _FireDirection);



	FORCEINLINE float GetCurrentScore() const { return CurrentScore; }
	//FORCEINLINE void SpendGold(float Amount) { Amount > GoldCount ? GoldCount += 0.0f : GoldCount -= Amount; GoldCount = FMath::Clamp(GoldCount, 0.0f, 99999.0f); }
	FORCEINLINE void AddToScore(int Amount) { CurrentScore += Amount; CurrentScore = FMath::Clamp(CurrentScore, 0, 99999999); }


};
