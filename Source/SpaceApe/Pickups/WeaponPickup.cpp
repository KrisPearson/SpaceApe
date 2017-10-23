// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "SpaceApePlayerCharacter.h"
#include "Components/GaussBlaster_PWComponent.h"
#include "Components/MissileLauncher_PWComponent.h"
#include "Components/BeamRifle_PWComponent.h"
#include "Components/PlayerWeaponComponent.h"

AWeaponPickup::AWeaponPickup() {


	UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup constructor w/o parameter"));

	WeaponType = UMissileLauncher_PWComponent::StaticClass();


	FString MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"); // better than this - load an icon and assign it to a dynamic material texture parameter?
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh(*MeshFilePath);
	//PickupMesh->StaticMesh = LoadedMesh.Object;
	PickupMesh->SetStaticMesh(LoadedMesh.Object);
	static ConstructorHelpers::FObjectFinder<USoundBase> LoadedSound(TEXT("SoundWave'/Game/TwinStick/Audio/KKIIDDZZ_00072.KKIIDDZZ_00072'"));
	PickUpSound = LoadedSound.Object;


	//AWeaponPickup(EWeaponPickupType(rand() % 10));
}

AWeaponPickup::AWeaponPickup(EWeaponPickupType _EWeaponType) {
	//WeaponType = UPlayerWeaponComponent();

	//WeaponPickupTypeEnum = _EWeaponType;

	UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup constructor w/ parameter"));



	EWeaponPickupType test = EWeaponPickupType(rand() % 10);


	FString MeshFilePath ;


	switch (test) {
	case EWeaponPickupType::WP_GaussBlaster :
		WeaponType = UGaussBlaster_PWComponent::StaticClass();
		MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"); // better than this - load an icon and assign it to a dynamic material texture parameter?
		//PickupMesh = 
		break;
	case EWeaponPickupType::WP_MissileLauncher:
		WeaponType = UMissileLauncher_PWComponent::StaticClass();
		MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'");
		break;
	case EWeaponPickupType::WP_BeamRifle:
		WeaponType = UBeamRifle_PWComponent::StaticClass();


		MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'");
		break;
	case EWeaponPickupType::WP_BombPitcher:
		WeaponType = UGaussBlaster_PWComponent::StaticClass();
		MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'");
		break;

	default :

		MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'");

		break;

	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh( *MeshFilePath );
	PickupMesh->StaticMesh = LoadedMesh.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> LoadedSound(TEXT("SoundWave'/Game/TwinStick/Audio/KKIIDDZZ_00113.KKIIDDZZ_00113'"));
	PickUpSound = LoadedSound.Object;


	//AWeaponPickup();
}

void AWeaponPickup::OnBeginOverlapAction(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ASpaceApePlayerCharacter* PlayerCharacter = Cast<ASpaceApePlayerCharacter>(OtherActor);
	
	UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup OnBeginOverlapAction"));

	if (PlayerCharacter != nullptr) {

		UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup OnBeginOverlapAction PlayerCharacter != nullptr"));

		//PlayerCharacter->PickupWeapon(weapontype);

		//UPlayerWeaponComponent NewWeaponComp = UPlayerWeaponComponent;

		PlayerCharacter->ChangeWeapon(WeaponType);
	}


	Super::OnBeginOverlapAction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

/* This is a nice approach. Use an override, and handle the destroy in the base class implementation - calling Super
AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
if (OnlineCharacter != nullptr)
{
// Add a key to players inventory or something
//
OnlineCharacter->GainKey();
UE_LOG(LogTemp, Warning, TEXT("%s has %d keys"), *OnlineCharacter->GetName(), OnlineCharacter->GetKeyCount());
// Sound - Effect - Destroy
Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

*/