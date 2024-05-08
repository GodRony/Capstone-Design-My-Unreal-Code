// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Heroes.h"
#include "Interfaces/Assassination.h"

#include "Main.generated.h"


/**
 * 
 */

UCLASS()
class WINTERPROJECT_API AMain : public AHeroes ,public IAssassination
{
	GENERATED_BODY()
public:
	AMain();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void AddSoul(class ASoul* Soul) override;

	bool IsAttacking();
protected:
	virtual void BeginPlay() override;

	virtual void AttackR(); // 기본공격 

	virtual void AttackG(); // 마법공격

	virtual void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) override;
	virtual void Die() override;
	virtual void GetAssassinated(class AActor* Victims) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	AActor* Victim;


private:

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

};
