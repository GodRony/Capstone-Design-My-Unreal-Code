// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UMyGameInstance : public UGameInstance 
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 ShinbiLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	int32 KwangLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float ShinbiHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float KwangHealth;
	UFUNCTION(BlueprintCallable)
	void SetShinbiLevel(int32 Level);
	UFUNCTION(BlueprintCallable)
	void SetKwangLevel(int32 Level);
};
