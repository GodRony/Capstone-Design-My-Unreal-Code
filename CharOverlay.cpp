// Fill out your copyright notice in the Description page of Project Settings.


#include "CharOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
void UCharOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
	//HealthProgressBar->SetPercent(Percent);	
}

void UCharOverlay::SetMPBarPercent(float Percent)
{
	if (MPProgressBar)
	{
		MPProgressBar->SetPercent(Percent);
		//	UE_LOG(LogTemp,Warning,TEXT("SetMPBarPercentCalled"))
	}
}

void UCharOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UCharOverlay::SetSoul(int32 Soul)
{
	if (SoulCount)
	{
		const FString String = FString::Printf(TEXT("%d"), Soul);
		const FText Text = FText::FromString(String);
		SoulCount->SetText(Text);
	}
}
