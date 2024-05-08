// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolToCombatArea.generated.h"

UCLASS()
class WINTERPROJECT_API APatrolToCombatArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolToCombatArea();
	UPROPERTY(EditAnywhere, Category = "GameMode")
	class UBoxComponent* BoxComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:


	//UPROPERTY(EditAnywhere, Category = "Sound")
	
};
