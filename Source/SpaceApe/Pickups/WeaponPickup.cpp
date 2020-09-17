// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "SpaceApePlayerCharacter.h"
#include "Components/GaussBlaster_PWComponent.h"
#include "Components/MissileLauncher_PWComponent.h"
#include "Components/BeamRifle_PWComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/BombLauncher_PWComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

AWeaponPickup::AWeaponPickup() {

	static ConstructorHelpers::FObjectFinder<USoundBase> LoadedSound(TEXT("SoundWave'/Game/TwinStick/Audio/KKIIDDZZ_00113.KKIIDDZZ_00113'"));
	PickUpSound = LoadedSound.Object;
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
	if (HasAuthority()) {
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
			WeaponType = UBombLauncher_PWComponent::StaticClass();

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