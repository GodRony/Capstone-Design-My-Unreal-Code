// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageUI.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UDamageUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void ShowDamageUI(float Damage);
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
	class UButton* DamageButton;

};
