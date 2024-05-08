// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(float Percent);	
private:
	class UHealthBar* HealthBarWidget;
};
