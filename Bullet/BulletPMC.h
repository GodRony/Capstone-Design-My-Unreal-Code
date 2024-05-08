// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BulletPMC.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API ABulletPMC : public AActor, public IHitInterface
{
	GENERATED_BODY()
public:
	ABulletPMC();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BulletComp")
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BulletComp")
	class USphereComponent* SphereComponent;
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter);

protected:
	//virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
