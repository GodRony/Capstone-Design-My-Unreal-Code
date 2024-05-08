
#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "UserWidget/DamageUI.h"
#include "Interfaces/Assassination.h"
#include "EnemyBase.generated.h"

/**
 *
 */
UCLASS()
class WINTERPROJECT_API AEnemyBase : public ABaseCharacter, public IAssassination
{
	GENERATED_BODY()
public:

	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	void DecreaseHealth(float Damage);
	UFUNCTION(BlueprintCallable)
	virtual void GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter) override;


protected:
	virtual void BeginPlay() override;

	/** </ABaseCharacter> **/
	UPROPERTY(BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	//	virtual void GetAssassinated(class AActor* Victims) override;
	virtual void GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute);
	virtual void ShowAimUI();
	virtual void HideAimUI();
	virtual void Attack() override;
	virtual void Die() override;
	virtual void SpawnSoul();
	virtual void PlayAssassinated() override;

	virtual void PlayHitReactMontage(const FName& SectionName) override;

	UFUNCTION(BlueprintCallable)
	void AttackBySelectedSkill();
	virtual void InitializeEnemy();


	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* SetTargetSkillParticle(class UNiagaraSystem* ParticleSystem);


	/// ////////////
	void HideHealthBar();
	void ShowHealthBar();

	bool IsDead();

	// Components //
	float DeathLifeSpan = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AssasinatedMontage;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UWidgetComponent* AimWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UParticleSystem* SkillParticle;

	FTimerHandle SpawnSoulTimerHandle;

	// UI


};
