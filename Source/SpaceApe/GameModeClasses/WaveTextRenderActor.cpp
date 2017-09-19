// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveTextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"


void AWaveTextRenderActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaveTextRenderActor, DisplayText);
}

void AWaveTextRenderActor::OnRep_TextUpdated() {
	if (WaveTextRenderComponent != nullptr) {
		//TextRender->SetText(DisplayText);
		WaveTextRenderComponent->SetText(DisplayText); // should store renderComponent
		WaveTextRenderComponent->SetVisibility(true);
	}
}

void AWaveTextRenderActor::BeginPlay() {
	Super::BeginPlay();

	WaveTextRenderComponent = GetTextRender();

	WaveTextRenderComponent->SetIsReplicated(true);
	WaveTextRenderComponent->SetWorldSize(400);
	WaveTextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	WaveTextRenderComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	WaveTextRenderComponent->SetVisibility(false);
}

