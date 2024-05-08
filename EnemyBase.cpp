// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon.h"
#include "Soul.h"
#include "Components/BoxComponent.h"
#include "PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HealthBarComponent.h"

AEnemyBase::AEnemyBase()
{
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	AimWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UsweWidget"));
	AimWidget->SetupAttachment(GetRootComponent());
	AimWidget->SetVisibility(false);
	//DamageWidget = CreateDefaultSubobject<UDamageUI>(TEXT("UDamageUI"));
	//DamageWidget->SetupAttachment(GetRootComponent());
	//DamageWidget->SetVisibility(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AEnemyBase::GetDamagedBySkill(AActor* Hitter, float Damage, ESkillAttribute SkillAttribute)
{
	Super::GetDamagedBySkill(Hitter, Damage, SkillAttribute);
	if (IsAlive() && Hitter) {
		if (!IsDead()) ShowHealthBar();
		UE_LOG(LogTemp, Warning, TEXT("%f"), DamageAmount);

		if (Hitter) CombatTarget = Hitter;
		DecreaseHealth(DamageAmount);
	}
	else {
		Die();

	}
}

void AEnemyBase::AttackBySelectedSkill()
{
	if (CombatTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyBase Target Is Valid"));
		IUseSkill* TargetBySkill = Cast<IUseSkill>(CombatTarget);
		if (TargetBySkill)
		{
			TargetBySkill->GetDamagedBySkill(this, SkillDamage, SelectedSkill);
		}
		//TargetBySkill->HideAimUI();
	}
}
void AEnemyBase::ShowAimUI()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase ShowAimUI Called"));

	AimWidget->SetVisibility(true);

}
void AEnemyBase::HideAimUI()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase HideAimUI Called"));
	AimWidget->SetVisibility(false);
}

void AEnemyBase::DecreaseHealth(float Damage)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(Damage);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}
void AEnemyBase::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontageRandomBasicR();
}

void AEnemyBase::InitializeEnemy()
{
	//EnemyController = Cast<AAIController>(GetController());
//	MoveToTarget(PatrolTarget);
	HideHealthBar();

}

void AEnemyBase::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemyBase::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}



UNiagaraComponent* AEnemyBase::SetTargetSkillParticle(UNiagaraSystem* ParticleSystem)
{
	UNiagaraComponent* SpawnedSystem = nullptr;
	if (CombatTarget)
	{

		SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ParticleSystem,
			CombatTarget->GetActorLocation()

		);
	}
	return SpawnedSystem;
}


void AEnemyBase::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Damage, Hitter);
	if (!IsDead()) ShowHealthBar();
	//ClearPatrolTimer();
	//ClearAttackTimer();

	if (Hitter) CombatTarget = Hitter;
	DecreaseHealth(Damage);
}

void AEnemyBase::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!Attributes->IsAlive())
	{
		Die();
		return;
	}
	if (Attributes && Attributes->IsAlive()) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}

}

void AEnemyBase::Die()
{
	Super::Die();
	//	UE_LOG(LogTemp, Warning, TEXT("Die Called"));
	if (IsDead()) return;
	EnemyState = EEnemyState::EES_Dead;
	DisableCapsule();
	HideHealthBar();
	SetLifeSpan(DeathLifeSpan);
	Tags.Add(FName("Dead"));
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	GetCharacterMovement()->bOrientRotationToMovement = false;

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(CombatTarget);
	if (PickupInterface)
	{
		PickupInterface->AddXP(Attributes->GetXP());
	}

}

void AEnemyBase::SpawnSoul()
{
}

void AEnemyBase::PlayAssassinated()
{
	this->PlayAnimMontage(AssasinatedMontage);
	//AssasinatedMontage->OnMontageEnded.AddDynamic(this, &AEnemy::Die);
}

bool AEnemyBase::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}


