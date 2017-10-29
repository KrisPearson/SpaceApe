// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "SpaceApePlayerCharacter.h"
#include "Components/GaussBlaster_PWComponent.h"
#include "Components/MissileLauncher_PWComponent.h"
#include "Components/BeamRifle_PWComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

AWeaponPickup::AWeaponPickup() {
	//WeaponType = UMissileLauncher_PWComponent::StaticClass();

	//FString MeshFilePath = TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"); // better than this - load an icon and assign it to a dynamic material texture parameter?
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh(*MeshFilePath);
	//PickupMesh->StaticMesh = LoadedMesh.Object;
	//PickupMesh->SetStaticMesh(LoadedMesh.Object);
	//static ConstructorHelpers::FObjectFinder<USoundBase> LoadedSound(TEXT("SoundWave'/Game/TwinStick/Audio/KKIIDDZZ_00072.KKIIDDZZ_00072'"));
	//PickUpSound = LoadedSound.Object;


	//ChooseWeaponPickup(EWeaponPickupType(rand() % 10));

	//EWeaponPickupType RandomWeaponType = EWeaponPickupType(rand() % 10);

	if (Role == ROLE_Authority) {
		/*
		EWeaponPickupType RandomWeaponType = static_cast<EWeaponPickupType>(FMath::RandRange(0, 3));


		FString MaterialFilePath;

		switch (RandomWeaponType) {
		case EWeaponPickupType::WP_GaussBlaster:
			WeaponType = UGaussBlaster_PWComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/GaussPickup_MatInstance.GaussPickup_MatInstance'");																  //PickupMesh = 

			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Gauss"));
			break;
		case EWeaponPickupType::WP_MissileLauncher:
			WeaponType = UMissileLauncher_PWComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/MissilePickup_MatInstance.MissilePickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Missile"));
			break;
		case EWeaponPickupType::WP_BeamRifle:
			WeaponType = UBeamRifle_PWComponent::StaticClass();


			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BeamPickup_MatInstance.BeamPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Beam"));
			break;
		case EWeaponPickupType::WP_BombPitcher:
			WeaponType = UPlayerWeaponComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BombPickup_MatInstance.BombPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Bomb"));
			break;

		default:

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BeamPickup_MatInstance.BeamPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = DEFAULT"));
			break;
		}

		//static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh(*MeshFilePath);		
		//PickupMesh->SetStaticMesh(LoadedMesh.Object);
		//StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *MaterialFilePath);
		//static ConstructorHelpers::FObjectFinder<UMaterialInstance> LoadedMaterial(*MaterialFilePath);
		//PickupMesh->SetMaterial(0, Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *MaterialFilePath)));

		LoadAndUpdateMaterial(MaterialFilePath);
		*/

		//UE_LOG(LogTemp, Warning, TEXT("AWeaponPickupPath = %s"), *MaterialFilePath);
		//UE_LOG(LogTemp, Warning, TEXT("AWeaponPickupName = %s"), *LoadedMaterial.Object->GetName());

		static ConstructorHelpers::FObjectFinder<USoundBase> LoadedSound(TEXT("SoundWave'/Game/TwinStick/Audio/KKIIDDZZ_00113.KKIIDDZZ_00113'"));
		PickUpSound = LoadedSound.Object;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup ROLE NOT AUTHORITY")); }
}


void AWeaponPickup::OnBeginOverlapAction(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ASpaceApePlayerCharacter* PlayerCharacter = Cast<ASpaceApePlayerCharacter>(OtherActor);
	
	//UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup OnBeginOverlapAction"));

	if (PlayerCharacter != nullptr) {

		//UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup OnBeginOverlapAction PlayerCharacter != nullptr"));

		//PlayerCharacter->PickupWeapon(weapontype);

		//UPlayerWeaponComponent NewWeaponComp = UPlayerWeaponComponent;

		PlayerCharacter->ChangeWeapon(WeaponType);
	}

	Super::OnBeginOverlapAction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeaponPickup::AssignRandomPickupType() {
	if (Role == ROLE_Authority) {
		EWeaponPickupType RandomWeaponType = static_cast<EWeaponPickupType>(FMath::RandRange(0, 3));

		FString MaterialFilePath;

		switch (RandomWeaponType) {
		case EWeaponPickupType::WP_GaussBlaster:
			WeaponType = UGaussBlaster_PWComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/GaussPickup_MatInstance.GaussPickup_MatInstance'");																  //PickupMesh = 

			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Gauss"));
			break;
		case EWeaponPickupType::WP_MissileLauncher:
			WeaponType = UMissileLauncher_PWComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/MissilePickup_MatInstance.MissilePickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Missile"));
			break;
		case EWeaponPickupType::WP_BeamRifle:
			WeaponType = UBeamRifle_PWComponent::StaticClass();


			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BeamPickup_MatInstance.BeamPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Beam"));
			break;
		case EWeaponPickupType::WP_BombPitcher:
			WeaponType = UPlayerWeaponComponent::StaticClass();

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BombPickup_MatInstance.BombPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = Bomb"));
			break;

		default:

			MaterialFilePath = TEXT("MaterialInstanceConstant'/Game/Materials/Pickups/BeamPickup_MatInstance.BeamPickup_MatInstance'");
			UE_LOG(LogTemp, Warning, TEXT("AWeaponPickup = DEFAULT"));
			break;
		}

		LoadAndUpdateMaterial(MaterialFilePath);
	}
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