// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterType.h"
#include "PickupInterface.h"
#include "GameInstance/MyGameInstance.h"
#include "Heroes.generated.h"

/**
 *
 */
UCLASS()
class WINTERPROJECT_API AHeroes : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHeroes();
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter) override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetOverlappingItem(class AItem* Item);
	virtual void AddSoul(class ASoul* Soul) override;
	virtual void AddXP(float XP) override;
	bool IsAttacking();


	// GameState
	void SetPatrolGameState();
	void SetCombatGameState();

	// CombatState
	void FindEnemy();
	UFUNCTION(BlueprintCallable)
	bool UseMP(float ComsumeMP);
	UFUNCTION(BlueprintCallable)
	bool UseStamina(float ComsumeStamina);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	void DisableCharOverlay();

	virtual void InitializeCharOverlay();


	// Input //
	APlayerController* PlayerController;
	class UEnhancedInputComponent* EnhancedPlayerInputComponent;
	class UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IC_Patrol;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IC_Combat;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharLook;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharMove;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharJump;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharAttackR; // Skill 선택 창 시 Yes
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharAttackG; // 공격 스타일 체인지
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharAttackT; // Skill 선택
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_CharShift; 
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PickUp;

	// Item 

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;
	//

	// Etc.. UI //

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UCharOverlay> CharOverlayWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UCharOverlay* CharOverlay;
	UPROPERTY(BlueprintReadWrite)
	float StaminaGaugeSpeed = 5.f;
	TArray<AActor*> IgnoreActors;
	UFUNCTION(BlueprintCallable)
	void HideTargetUI();
	UFUNCTION(BlueprintCallable)
	void ShowTargetUI();
	UPROPERTY(BlueprintReadWrite, Category = Combat)
	bool IsCombat = false;


	// Enum class 
	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY(BlueprintReadWrite)
	EAttackState AttackState = EAttackState::EATS_SwordState;

	// Func
	virtual void Look(const FInputActionValue& Value);
	virtual void Move(const FInputActionValue& Value);
	virtual void AttackR(); // 기본공격
	UFUNCTION(BlueprintCallable)
	virtual void AttackG(); // 공격 스타일 체인지
	UFUNCTION(BlueprintCallable)
	virtual void SelectEnemy();
	UFUNCTION(BlueprintCallable)
	void AttackBySelectedSkill();

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* SetTargetSkillParticle(class UNiagaraSystem* ParticleSystem);
	virtual void GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute);


	virtual void Shift();
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) override;
	virtual void Die() override;
	virtual void PickUp();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();


	// AI State 
	class AAIController* AIController;

	void PossessedBy(AController* NewController);
	void UnPossessed();
	APawn* PlayerPossessedPawn;

	EAIPossessState AIPossessState = EAIPossessState::EPS_AIPossess;
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.1f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 0.3f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackRadius = 75.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float RotationSpeed = 0.1f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float MinDistanceToEnemy = 2000.f;

	virtual AActor* FindClosestEnemy();
	virtual void StartAttackTimer();
	virtual void ClearAttackTimer();
	virtual void ChaseTarget();
	virtual void TargetIsDead();
	
	UPROPERTY(BlueprintReadWrite, Category = Combat)
	bool IsDead = false;

	UPROPERTY(BlueprintReadWrite, Category = Combat)
	AActor* Target;

	UPROPERTY(BlueprintReadWrite, Category = Combat)
	TArray<AActor*> Enemies;

	UPROPERTY(EditAnywhere, Category = AIState)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = AIState)
	float MoveToPlayerPossessPawnDist = 100.f;

	// Character Speed
	UPROPERTY(EditAnywhere, Category = AIState)
	float ChasePlayerSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = AIState)
	float PossessedSpeed = 600.f;

	UFUNCTION(BlueprintCallable)
	void SetHidden();

	bool AreAllEnemiesDead = false;
	float EnemyToHeoresDist = 100.0f;


	// Camera

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* springArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CamComp;


	// ++ Heroes Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ShiftMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ChangeStyleToUnique;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ChangeStyleToBasic;

	// Skill Ability - Heal
	UFUNCTION(BlueprintCallable)
	void UseSkillHeal();
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float HealAmount = 30.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float SwordDamage = 20.f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponTraceEnd;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* WeaponBox;

private:


};
