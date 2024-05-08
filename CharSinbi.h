
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Heroes.h"
#include "CharSinbi.generated.h"

// CharSinbi������ Weapon Ŭ������ ������� �ʰ� Weapon�� ����� ����
// ������ Skeletal Mesh�� �̹� Weapon�� �ٿ��� �ֱ� ������.. 
UCLASS()
class WINTERPROJECT_API ACharSinbi : public AHeroes
{
	GENERATED_BODY()
public:

	ACharSinbi();
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter) override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void AddSoul(class ASoul* Soul) override;

protected:
	virtual void BeginPlay() override;

	virtual void AttackR(); // �⺻���� 
	virtual void AttackG(); // ��������


	virtual void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) override;
	virtual void Die() override;
	virtual void Shift() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Skill)
	float DashDistance = 500.f;

	UFUNCTION(BlueprintCallable)
	void ShootingBullet();
	UFUNCTION(BlueprintCallable)
	void StartShootingProjectile();
	UFUNCTION(BlueprintCallable)
	void EndShootingProjectile();
	bool IsShootingProjectile = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bullet)
	TSubclassOf<class ABulletPMC> BulletClass;
	float BulletDamage;




private:

	UFUNCTION()
		void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE EAttackState GetAttackState() const { return AttackState; }
};
