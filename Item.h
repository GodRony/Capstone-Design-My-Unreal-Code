// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};
UCLASS()
class WINTERPROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void SpawnPickupEffect();
	virtual void SpawnPickupSound();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	EItemState ItemState = EItemState::EIS_Hovering;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = Time)
	float RunningTime;
	UPROPERTY(EditAnywhere, Category = Time)
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeConstant = 5.f;
	UPROPERTY(EditAnywhere, Category = Effect)
		class UNiagaraComponent* ItemEffect;
	UPROPERTY(EditAnywhere, Category = Effect)
		USoundBase* PickupSound;

private:


};
