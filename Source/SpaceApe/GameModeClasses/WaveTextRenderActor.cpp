// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveTextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"


void AWaveTextRenderActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AWaveTextRenderActor::DisplayNextWaveMessage(int _WaveNumber) {
	MulticastStartWaveTimer(_WaveNumber);
}

void AWaveTextRenderActor::MulticastStartWaveTimer_Implementation(int _WaveNumber) {

	FString LeadingZeros;
	if (_WaveNumber < 10) LeadingZeros = "00";
	else if (_WaveNumber < 100)LeadingZeros = "0";
	else LeadingZeros = "";
	WaveTextString = FString("LEVEL" + LeadingZeros + FString::FromInt(_WaveNumber));

	GetWorld()->GetTimerManager().SetTimer(WaveTextTimerHandle, this, &AWaveTextRenderActor::WaveTextTimerFunc, 0.1f, true);
}

void AWaveTextRenderActor::WaveTextTimerFunc() {

	const FString StringToDisplay = WaveTextString.Mid(0, WaveTextStringIndex);

	DisplayText = FText::AsCultureInvariant(StringToDisplay);

	WaveTextRenderComponent->SetText(DisplayText);

	WaveTextRenderComponent->SetVisibility(true);


	WaveTextStringIndex++;

	if (WaveTextStringIndex == 9) {
		GetWorld()->GetTimerManager().SetTimer(WaveTextTimerHandle, this, &AWaveTextRenderActor::WaveTextTimerFunc, 1.f, false);
	}
	if (WaveTextStringIndex == 10) {
		WaveTextRenderComponent->SetVisibility(false);
		WaveTextStringIndex = 0;
	}
}

void AWaveTextRenderActor::BeginPlay() {
	Super::BeginPlay();

	bReplicates = true;

	WaveTextRenderComponent = GetTextRender();

	WaveTextRenderComponent->SetIsReplicated(true);
	WaveTextRenderComponent->SetWorldSize(400);
	WaveTextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	WaveTextRenderComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	WaveTextRenderComponent->SetVisibility(false);
}

