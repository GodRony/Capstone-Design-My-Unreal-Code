// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

UCLASS()
class WINTERPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* newInstigator);
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool ActorIsSameType(AActor* OtherActor);
	void ExecuteGetHit(FHitResult& BoxHit);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	//UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	//USoundBase* EquipSound;
	void BoxTrace(FHitResult& BoxHit);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		FVector BoxTraceExtent = FVector(5.f);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		bool bShowBoxDebug = false;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class UBoxComponent* WeaponBox;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Damage = 20.f;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};