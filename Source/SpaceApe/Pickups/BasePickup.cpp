// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceApePlayerCharacter.h"


// Sets default values
ABasePickup::ABasePickup()
{
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	RootComponent = MyRoot;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	//PickupMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	//PickupMesh->SetupAttachment(RootComponent);
	//PickupMesh->BodyInstance.SetCollisionProfileName("Projectile"); // should be pickup
	PickupMesh->bGenerateOverlapEvents = true;
	PickupMesh->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnBeginOverlap); // set up a notification for when this component hits something
	PickupMesh->SetupAttachment(MyRoot);

	FString MeshFilePath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"); // better than this - load an icon and assign it to a dynamic material texture parameter?
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh(*MeshFilePath);

	PickupMesh->SetStaticMesh(LoadedMesh.Object);
	PickupMesh->BodyInstance.SetCollisionProfileName("Pickup");

	bReplicates = true;
}

void ABasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePickup, PickUpEffect);
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();

	AssignRandomPickupType();
	
}

void ABasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	OnBeginOverlapAction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//UE_LOG(LogTemp, Log, TEXT(" ABasePickup OnHit"));
	//if (OtherActor->GetClass()->IsChildOf(ASpaceApePlayerCharacter::StaticClass())) {
	//	Cast<ASpaceApePlayerCharacter>(OtherActor)->CollectPickup(this);
	//}
}

void ABasePickup::OnBeginOverlapAction(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	// do we need to perform some kind of rpc here?
	
	if (OtherActor->IsA(ASpaceApePlayerCharacter::StaticClass() )) {
		if (HasAuthority())
		{
			// Server
			UE_LOG(LogTemp, Warning, TEXT("Trigger Activated - Server, : %s"), *OtherActor->GetName());

			// Adding in possiblity to move effect through code if lacking animations
			// And doppler the sound but probably wont use it.
			const FVector SpawnLocation = GetActorLocation();
			PlayPickUpSound(SpawnLocation);
			HandleOverlap();
		}
		else
		{
			// Client

			UE_LOG(LogTemp, Warning, TEXT("Trigger Activated - Client, : %s"), *OtherActor->GetName());

			// Adding in possiblity to move effect through code if lacking animations
			// And doppler the sound but probably wont use it.
			const FVector SpawnLocation = GetActorLocation();
			PlayPickUpSound(SpawnLocation);
			HandleOverlap();
			//PlayPickUpEffect(SpawnLocation);
			//// Might wanna delay this by 1s, and just "Fake Destroy" StaticMesh, we'll see
			//Destroy();

			//ServerHandleOverlap();
		}
	}
}

void ABasePickup::LoadAndUpdateMaterial_Implementation(const FString& FilePath) {
	UE_LOG(LogTemp, Warning, TEXT("LoadAndUpdateMaterial_Implementation = %s"), *FilePath);
	PickupMesh->SetMaterial(0, Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *FilePath)));
}

void ABasePickup::AssignRandomPickupType() {
}

void ABasePickup::HandleOverlap() {
	const FVector SpawnLocation = GetActorLocation();
	PlayPickUpEffect(SpawnLocation);
	// Might wanna delay this by 1s, and just "Fake Destroy" StaticMesh, we'll see
	if (PickupMesh != nullptr)
	{
		PickupMesh->SetHiddenInGame(true);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//Destroy();
}

void ABasePickup::ServerHandleOverlap_Implementation() {
	HandleOverlap();
}

bool ABasePickup::ServerHandleOverlap_Validate() {
	return true;
}

void ABasePickup::PlayPickUpSound(const FVector& Location) {
	if (PickUpSound == nullptr) return;
	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, Location, SoundVolume);
}

void ABasePickup::PlayPickUpEffect(const FVector& Location) {
	if (PickUpEffect == nullptr) return;
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpEffect, Location);
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


