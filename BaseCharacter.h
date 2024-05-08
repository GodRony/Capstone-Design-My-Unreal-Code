// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterType.h"
#include "UserWidget/DamageUI.h"
#include "Interfaces/UseSkill.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class WINTERPROJECT_API ABaseCharacter : public ACharacter, public IHitInterface, public IUseSkill
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;
protected:
	virtual void BeginPlay() override;

	// Combat //
	virtual void Attack();
	virtual void Die();
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter);
	void HitEffect(AActor* Hitter, const FVector& ImpactPoint);
	virtual void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual int32 PlayAttackMontageRandomBasicR();
	virtual int32 PlayAttackMontageRandomUniqueR();
	virtual int32 PlaySkillMontageRandom();
	virtual int32 PlayDeathMontage();

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);



	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();


	UPROPERTY(BlueprintReadWrite, Category = Combat)
	AActor* CombatTarget;


	virtual bool CanAttack();
	bool IsAlive();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* AttackMontageBasicR; // 기본공격
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* AttackMontageUniqueR; // 어빌리티 공격
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* AttackMontageChangeStyleG;
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* SkillMontage; // 스킬 공격
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DamagedBySkillMontage;

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	UFUNCTION(BlueprintCallable)
	virtual void GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute);

	UPROPERTY(EditAnywhere, Category = SkillAttribute)
	ESkillAttribute Weak;
	UPROPERTY(EditAnywhere, Category = SkillAttribute)
	ESkillAttribute Block;
	UPROPERTY(EditAnywhere, Category = SkillAttribute)
	ESkillAttribute Available;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillAttribute)
	ESkillAttribute SelectedSkill;

	float DamageAmount;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageUI> DamageWidgetClass;
	UPROPERTY(EditAnywhere, Category = Stat)
	float SkillDamage = 20;

private:

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;


	UPROPERTY(EditAnywhere, Category = MotionWarping)
	double WarpTargetDistance = 75.f;

	UPROPERTY(EditDefaultsOnly, Category = MontageSection)
	TArray<FName> AttackMontageSectionsBasicR; // 기본공격
	UPROPERTY(EditDefaultsOnly, Category = MontageSection)
	TArray<FName> AttackMontageSectionsUniqueR;
	UPROPERTY(EditDefaultsOnly, Category = MontageSection)
	TArray<FName> SkillMontageSections; // 스킬공격
	UPROPERTY(EditDefaultsOnly, Category = MontageSection)
	TArray<FName> DeathMontageSections;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
