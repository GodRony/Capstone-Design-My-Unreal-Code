// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharOverlay.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UCharOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBarPercent(float Percent);
	void SetMPBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	//void SetGold(int32 Gold);
	void SetSoul(int32 Soul);
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MPProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;

//	UPROPERTY(meta = (BindWidget))
//	class UTextBlock* GoldCount;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SoulCount;


};
