// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "CharacterType.h"
#include "Interfaces/Assassination.h"
#include "Enemy.generated.h"

UCLASS()
class WINTERPROJECT_API AEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	/** <AActor> **/
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	void DecreaseHealth(float Damage);
	/** </AActor> **/

	/** <IHitInterface> **/
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter) override;

	/** </IHitInterface> **/
protected:
	/** <AActor> **/
	virtual void BeginPlay() override;
	/** </AActor> **/

	/** <ABaseCharacter> **/
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void Die() override;
	void SpawnSoul();
	void AttackEnd() override;
	virtual void PlayHitReactMontage(const FName& SectionName) override;


	//	virtual void GetAssassinated(class AActor* Victims) override;

	virtual void PlayAssassinated() override;
	virtual void GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute);
private:

	// AI behavior //
	void InitializeEnemy();

	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();

	bool IsChasing();
	bool IsAttacking();

	bool IsEngaged();
	bool IsAlive();


	bool InTargetRange(AActor* Target, double Radius);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideCombatRadius();
	bool IsInsideAttackRadius();

	void MoveToTarget(AActor* Target);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	AActor* ChoosePatrolTarget();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	void PatrolTimerFinished();
	void SpawnDefaultWeapon();
	////////////////////////

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	// Components //
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 3.f;


	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	FTimerHandle PatrolTimer;
	FTimerHandle SpawnSoulTimerHandle;
	class AAIController* EnemyController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* VictimBox;
	UFUNCTION()
	void OnVictimBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnVictimBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
