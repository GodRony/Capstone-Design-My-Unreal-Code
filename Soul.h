// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API ASoul : public AItem
{
	GENERATED_BODY()
public:


protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere)
		int32 Soul;
public:
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE void SetSoul(int32 NumberOfSouls) { Soul = NumberOfSouls; }
};
