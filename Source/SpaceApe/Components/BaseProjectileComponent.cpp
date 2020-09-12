// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectileComponent.h"


/*
The Projectile Component ( powerup? ) could:
	> attach to weapon or projectile, depending on the type of powerup. Some kind of dictionary approach could be used here?
	> bind to events such as on hit(projectile) or shoot(weaponcomponent).




*/


// Sets default values for this component's properties
UBaseProjectileComponent::UBaseProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

