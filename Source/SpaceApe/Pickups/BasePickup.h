// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class SPACEAPE_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"), Replicated)
		UStaticMeshComponent* PickupMesh;


protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Audio feedback - for when you pick up an item - could replicate this sound but might not*/
	UPROPERTY(EditAnywhere, Category = "C++ Base Class", Replicated)
		USoundBase* PickUpSound;

	/** Visual feedback - for when you pick up an item - should replicate probably*/
	UPROPERTY(EditAnywhere, Category = "C++ Base Class", Replicated)
		UParticleSystem* PickUpEffect;

	/** Volume of sound */
	UPROPERTY(EditAnywhere, Category = "C++ Base Class")
		float SoundVolume = 0.25f;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	/* Overridable function to do something when overlapped*/
	virtual void OnBeginOverlapAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
		void LoadAndUpdateMaterial(const FString& FilePath);
	void LoadAndUpdateMaterial_Implementation(const FString& FilePath);

	virtual void AssignRandomPickupType();

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Base Class")
		USceneComponent* MyRoot;

	/* The colission function that gets called on all derived classes. */
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void HandleOverlap();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerHandleOverlap();

	void PlayPickUpSound(const FVector& Location);

	void PlayPickUpEffect(const FVector& Location);
};
