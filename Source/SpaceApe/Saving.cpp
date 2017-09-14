// Fill out your copyright notice in the Description page of Project Settings.

#include "Saving.h"
#include "UnrealNetwork.h"

void USaving::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USaving, PlayerSettings);
}

