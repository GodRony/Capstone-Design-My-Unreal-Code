// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon.h"
#include "Soul.h"
#include "Components/BoxComponent.h"
#include "HealthBarComponent.h"

#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
	PawnSensing->bOnlySensePlayers = false;



	VictimBox = CreateDefaultSubobject<UBoxComponent>(TEXT("VictimBox"));
	VictimBox->SetupAttachment(GetRootComponent());
	VictimBox->SetBoxExtent(FVector(110.f, 130.f, 90.f));
	VictimBox->SetWorldLocation(FVector(-100.f, 0.f, 0.f));
	//if(VictimBox){UE_LOG(LogTemp,Warning,TEXT("Victim(Initial) Exist")) }
	if (VictimBox) {
		VictimBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVictimBoxOverlap);

	}
}



// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitializeEnemy();
	Tags.Add(FName("Enemy"));
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	//VictimBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVictimBoxOverlap);
}



void AEnemy::InitializeEnemy()
{
	Super::InitializeEnemy();
	EnemyController = Cast<AAIController>(GetController());
	SpawnDefaultWeapon();
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) {
		//	UE_LOG(LogTemp, Warning, TEXT("Tick :: Dead Return"));
		return;
	}
	if (EnemyState > EEnemyState::EES_Patrolling) {
		//	UE_LOG(LogTemp, Warning, TEXT("Tick :: Will Chasing"));
		CheckCombatTarget();
	}
	else {
		//	UE_LOG(LogTemp, Warning, TEXT("Tick :: CheckPatrolTarget called"));
		CheckPatrolTarget();
	}

}

void AEnemy::GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute)
{
	Super::GetDamagedBySkill(Hitter, Damage, SkillAttribute);

	ClearPatrolTimer();
	ClearAttackTimer();
}

void AEnemy::DecreaseHealth(float Damage)
{
	Super::DecreaseHealth(Damage);

}
void AEnemy::Attack()
{
	Super::Attack();
}


bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}


void AEnemy::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{

	Super::GetHit(ImpactPoint, Damage, Hitter);

	ClearPatrolTimer();
	ClearAttackTimer();

}




void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	Super::PlayHitReactMontage(SectionName);
}




void AEnemy::Die()
{
	Super::Die();
	ClearAttackTimer();
	DisableCapsule();

	//	GetWorld()->GetTimerManager().SetTimer(SpawnSoulTimerHandle, this, &AEnemy::SpawnSoul, 2.0f);
}

void AEnemy::AttackEnd()
{

	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		//	ASoul* SpawnSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		//	SpawnSoul->SetSoul(Attributes->GetSoul());
	}
}


bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	// AI 캐릭터나 AI 컨트롤러에 대한 이동 명령
	// 이동할 위치, 회전, 속도 등을 포함하며, 이동 요청을 AI 시스템에 전달하여 경로 탐색 및 이동을 시작
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f); //  AI가 목표 지점에 얼마나 가까이 와야 하는지를 결정
	EnemyController->MoveTo(MoveRequest);

}
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}


void AEnemy::CheckCombatTarget()
{

	if (IsOutsideCombatRadius())
	{
		//	UE_LOG(LogTemp, Warning, TEXT("CheckCombatTarget :: IsOutsideCombatRadius"));
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}

	else if (IsOutsideAttackRadius() && !IsChasing())
	{

		//	UE_LOG(LogTemp, Warning, TEXT("CheckCombatTarget :: IsOutsideAttackRadius,  Chasing"));
		ClearAttackTimer();
		ChaseTarget();
		if (!IsEngaged()) ChaseTarget();

	}
	else if (CanAttack())
	{
		//	UE_LOG(LogTemp, Warning, TEXT("CheckCombatTarget :: CanAttack()"));

		StartAttackTimer();
		//	Attack();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) {
		//		UE_LOG(LogTemp, Warning, TEXT("Func CheckPatrolTarget Called"));
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f);
	}
}




void AEnemy::PawnSeen(APawn* SeenPawn)
{
	//	UE_LOG(LogTemp, Warning, TEXT("PawnSeen Called"));
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("Engagealbe Target"));
	if (bShouldChaseTarget)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("PawnSeen called"));
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AEnemy::OnVictimBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor->ActorHasTag("Main"))
	{
		//Tags.Add(FName("Victim"));

		/*IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint, Damage, GetOwner());
		}*/
		IAssassination* HitInterface = Cast<IAssassination>(OtherActor);
		if (HitInterface)
		{
			HitInterface->GetAssassinated(this);

		}


	}
}



void AEnemy::OnVictimBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Main"))
	{
		//Tags.Remove(FName("Victim"));
	}
}

/*void GetAssassinated(AActor* Victims)
{
	UE_LOG(LogTemp, Warning, TEXT("ssibal "));
}*/

void AEnemy::PlayAssassinated()
{
	Super::PlayAssassinated();
}


void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}
// ---------  ** AI behavior ** ---------  //

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsInsideCombatRadius()
{
	return InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}


bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}


void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}


bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}



bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

//---------------------------------------//
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float	AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::PatrolTimerFinished()
{
	//UE_LOG(LogTemp, Warning, TEXT("PatrolTimerFinished Called"));
	MoveToTarget(PatrolTarget);
}