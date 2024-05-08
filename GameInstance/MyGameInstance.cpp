// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CoreUObject.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"


void UMyGameInstance::SetShinbiLevel(int32 Level)
{
	ShinbiLevel = Level;
}

void UMyGameInstance::SetKwangLevel(int32 Level)
{
	KwangLevel = Level;
}
