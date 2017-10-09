// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePickup.h"
#include "SpaceApePlayerCharacter.h"


// Sets default values
ABasePickup::ABasePickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	//PickupMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	//PickupMesh->SetupAttachment(RootComponent);
	//PickupMesh->BodyInstance.SetCollisionProfileName("Projectile"); // should be pickup
	PickupMesh->OnComponentHit.AddDynamic(this, &ABasePickup::OnHit); // set up a notification for when this component hits something
	RootComponent = PickupMesh;
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePickup::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	UE_LOG(LogTemp, Log, TEXT(" ABasePickup OnHit"));
	if (OtherActor->GetClass()->IsChildOf(ASpaceApePlayerCharacter::StaticClass())) {
		Cast<ASpaceApePlayerCharacter>(OtherActor)->CollectPickup(this);
	}
}


