// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPWComponent.h"

void ULaserPWComponent::Shoot(FVector _FireDirection) {
	UE_LOG(LogTemp, Warning, TEXT("Laser UPlayerWeaponComponent firing"));
}

void ULaserPWComponent::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Laser ULaserPWComponent spawned"));
}
