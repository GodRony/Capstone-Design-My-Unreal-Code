// Fill out your copyright notice in the Description page of Project Settings.


#include "Heroes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "AttributeComponent.h"
#include "CharOverlay.h"
#include "Weapon.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Soul.h"

AHeroes::AHeroes()
{
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(springArmComp);
	CamComp->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponSphere"));
	WeaponBox->SetupAttachment(GetMesh(), FName("hand_r"));
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace Start"));
	WeaponTraceStart->SetupAttachment(GetMesh(), FName("Sword"));
	WeaponTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace End"));
	WeaponTraceEnd->SetupAttachment(GetMesh(), FName("Sword_Tip"));



}

void AHeroes::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Damage, Hitter);
	ClearAttackTimer();
	if (IsAlive())
	{
		Attributes->ReceiveDamage(Damage);
		CharOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		ActionState = EActionState::EAS_HitReaction;
	}
}
bool AHeroes::UseMP(float ComsumeMP)
{
	if (Attributes->UseMP(ComsumeMP)) {
		CharOverlay->SetMPBarPercent(Attributes->GetMPPercent());
		return true;
	}
	return false;
}
bool AHeroes::UseStamina(float ComsumeStamina)
{
	if (Attributes->UseStamina(ComsumeStamina))
	{
		CharOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		return true;
	}
	return false;
}
void AHeroes::SetPatrolGameState()
{
	IsCombat = false;
	DisableCharOverlay();
	EnhancedSubsystem->RemoveMappingContext(IC_Combat);
	EnhancedSubsystem->AddMappingContext(IC_Patrol, 1);
}

void AHeroes::SetCombatGameState()
{

	IsCombat = true;
	EnhancedSubsystem->RemoveMappingContext(IC_Patrol);
	EnhancedSubsystem->AddMappingContext(IC_Combat, 1);


	CharOverlay->AddToViewport();
}




void AHeroes::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Engagealbe Target"));
	GetMovementComponent()->Activate();
	AIController = GetWorld()->SpawnActor<AAIController>();
	// `Enemy` 태그를 가진 모든 액터를 찾습니다.


}
void AHeroes::SelectEnemy()
{
	Target = nullptr;
	FindEnemy();
	//AttackBySelectedSkill();
	if (Target)
	{
		IUseSkill* TargetUI = Cast<IUseSkill>(Target);
		IUseSkill* TargetBySkill = Cast<IUseSkill>(Target);

	}
}

void AHeroes::FindEnemy()
{
	//TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), Enemies);

	if (Enemies.Num() > 0)
	{
		TArray<AActor*> CloseEnemies;

		for (AActor* Enemy : Enemies)
		{
			// 현재 Actor와의 거리 계산
			float Distance = FVector::DistSquared(GetActorLocation(), Enemy->GetActorLocation());

			// 거리가 2000보다 작으면 Enemies 배열에 추가
			if (Distance < MinDistanceToEnemy * MinDistanceToEnemy)
			{
				CloseEnemies.Add(Enemy);
			}
		}
		// 거리가 2000보다 작은 적들만을 사용
		Enemies = CloseEnemies;
		if (Enemies.Num() == 0)
		{
			return;
		}
		Target = FindClosestEnemy();
	}
	if (Enemies.Num() <= 0) {
		AreAllEnemiesDead = true;
	}
}

AActor* AHeroes::FindClosestEnemy()
{
	AActor* ClosestEnemy = nullptr;


	if (Enemies.Num() <= 0)
	{
		//return nullptr;
	}
	for (int i = 0; i < Enemies.Num() - 1; ++i)
	{
		bool Swapped = false;
		for (int j = 0; j < Enemies.Num() - i - 1; ++j)
		{
			float Distance1 = FVector::DistSquared(GetActorLocation(), Enemies[j]->GetActorLocation());
			float Distance2 = FVector::DistSquared(GetActorLocation(), Enemies[j + 1]->GetActorLocation());

			if (Distance1 > Distance2)
			{
				Swap(Enemies[j], Enemies[j + 1]);
				Swapped = true;
			}
		}

		if (!Swapped)
		{
			break;
		}
	}
	return Enemies[0];
}

void AHeroes::DisableCharOverlay()
{
	if (CharOverlay->IsInViewport())
	{
		CharOverlay->RemoveFromParent();
	}
}

void AHeroes::InitializeCharOverlay()
{
	if (CharOverlayWidgetClass) {
		CharOverlay = CreateWidget<UCharOverlay>(GetWorld(), CharOverlayWidgetClass);
		CharOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		CharOverlay->SetMPBarPercent(Attributes->GetMPPercent());
		CharOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		CharOverlay->SetSoul(0);
		//	CharOverlay->AddToViewport();

	}


}
void AHeroes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	if (Attributes->Stamina <= Attributes->MaxStamina)
	{
		// 1초마다 Stamina 10씩 증가
		Attributes->Stamina += StaminaGaugeSpeed * DeltaTime;

		Attributes->Stamina = FMath::Min(Attributes->Stamina, Attributes->MaxStamina);

	}

	if (ActionState == EActionState::EAS_Dead)
	{

		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Tick :: No Dead"));
	if (!Target && AIPossessState == EAIPossessState::EPS_AIPossess)
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		PlayerPossessedPawn = PlayerController->GetPawn();
		//AIController->MoveToLocation(PlayerPossessedPawn->GetActorLocation());
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PlayerPossessedPawn);
		MoveRequest.SetAcceptanceRadius(MoveToPlayerPossessPawnDist); //  AI가 목표 지점에 얼마나 가까이 와야 하는지를 결정
		AIController->MoveTo(MoveRequest);
		if (IsDead == true)
		{
			ActionState = EActionState::EAS_Dead;
			return;
		}
		return;
	}
	if (AreAllEnemiesDead == true)
	{
		return;
	}
	if (Target && AIPossessState == EAIPossessState::EPS_AIPossess)
	{
		// 주인공 액터에서 대상까지의 방향을 계산하고 정규화
		FVector Direction = Target->GetActorLocation() - GetActorLocation();

		if (Target->ActorHasTag("Dead")) {
			Target = nullptr;
			ActionState = EActionState::EAS_Unoccupied;
			FindEnemy();
			if (IsDead == true)
			{
				ActionState = EActionState::EAS_Dead;
				return;
			}
			return;

		}

		if (Direction.Length() <= AttackRadius)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Main Tick : Attack"));
			//SetActorLocation();
			Direction.Z = 0.0f; // Z 축을 무시하거나 원하지 않으면 이 부분을 생략할 수 있습니다.
			Direction.Normalize();

			// 방향 벡터를 회전 각도로 변환하여 캐릭터의 회전을 설정
			FRotator TargetRotation = Direction.Rotation();
			FRotator NewRotation = FQuat::Slerp(GetActorRotation().Quaternion(), TargetRotation.Quaternion(), DeltaTime * RotationSpeed).Rotator();
			SetActorRotation(NewRotation);
			AttackR();
			//StartAttackTimer();
			ActionState = EActionState::EAS_Attacking;
		}

		if (AIController && Direction.Length() > AttackRadius && ActionState != EActionState::EAS_Attacking) // ChaseTarget
		{
			//UE_LOG(LogTemp, Warning, TEXT("Heroes Tick : ChaseTarget"));
			AIController->MoveToLocation(Target->GetActorLocation());
		}

	}
}

void AHeroes::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedPlayerInputComponent != nullptr) {
		PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr) {
			EnhancedSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (EnhancedSubsystem != nullptr) {
				EnhancedSubsystem->AddMappingContext(IC_Patrol, 1);
			}
		}

		EnhancedPlayerInputComponent->BindAction(IA_CharLook, ETriggerEvent::Triggered, this, &AHeroes::Look);
		EnhancedPlayerInputComponent->BindAction(IA_CharMove, ETriggerEvent::Triggered, this, &AHeroes::Move);
		EnhancedPlayerInputComponent->BindAction(IA_CharJump, ETriggerEvent::Started, this, &AHeroes::Jump);
		EnhancedPlayerInputComponent->BindAction(IA_CharJump, ETriggerEvent::Completed, this, &AHeroes::StopJumping);
		//CurrentGameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


		EnhancedPlayerInputComponent->BindAction(IA_CharShift, ETriggerEvent::Completed, this, &AHeroes::Shift);
		EnhancedPlayerInputComponent->BindAction(IA_CharAttackR, ETriggerEvent::Completed, this, &AHeroes::AttackR);
		EnhancedPlayerInputComponent->BindAction(IA_CharAttackG, ETriggerEvent::Completed, this, &AHeroes::AttackG);
		EnhancedPlayerInputComponent->BindAction(IA_PickUp, ETriggerEvent::Completed, this, &AHeroes::PickUp);


	}
}

void AHeroes::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AHeroes::AddSoul(ASoul* Soul)
{
	//	UE_LOG(LogTemp, Warning, TEXT("Add Souls Called"));
	if (Attributes && CharOverlay)
	{
		Attributes->AddSoul(Soul->GetSoul());
		CharOverlay->SetSoul(Attributes->GetSoul());
		//	UE_LOG(LogTemp, Warning, TEXT("Soul : %s"), Attributes->GetSoul());
	}
}
void AHeroes::AddXP(float XP)
{
	if (Attributes)
	{
		Attributes->AddXP(XP);
	}
}
void AHeroes::Die()
{
	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayDeathMontage();
	//SetLifeSpan(1.f);
	IsDead = true;

}

void AHeroes::PickUp()
{
	//UE_LOG(LogTemp, Warning, TEXT("Heroes : PickUp Called"));
	if (OverlappingItem) {
		UWorld* World = GetWorld();
		AWeapon* DefaultWeapon = Cast<AWeapon>(OverlappingItem);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
		if (EquippedWeapon)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("EquippedWeapon Exist"));
		}
	}
}




bool AHeroes::IsAttacking()
{
	return ActionState == EActionState::EAS_Attacking;
}




void AHeroes::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	if (InputValue.X != 0.0f) {
		AddControllerYawInput(InputValue.X);
	}
	if (InputValue.Y != 0.0f) {
		AddControllerPitchInput(InputValue.Y);
	}
}

void AHeroes::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	if (Controller != nullptr && (InputValue.X != 0.0f || InputValue.Y != 0.0f) && !IsAttacking()) {
		//const FVector RightDirection = UKismetMathLibrary::GetRightVector
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		if (InputValue.X != 0.0f) {
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			AddMovementInput(RightDirection, InputValue.X);
		}
		if (InputValue.Y != 0.0f) {
			const FVector ForwardDierection = YawRotation.Vector();
			AddMovementInput(ForwardDierection, InputValue.Y);
		}
	}
}

void AHeroes::AttackR()
{
	if (CanAttack())
	{
		if (AttackState == EAttackState::EATS_SwordState)
		{
			PlayAttackMontageRandomBasicR();
			ActionState = EActionState::EAS_Attacking;
		}
		if (AttackState == EAttackState::EATS_UniqueAbilityState)
		{
			PlayAttackMontageRandomUniqueR();
			ActionState = EActionState::EAS_Attacking;
		}
	}

}

void AHeroes::AttackG()
{
	if (AttackState == EAttackState::EATS_SwordState)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("EATS_SwordState"));
		AttackState = EAttackState::EATS_UniqueAbilityState;
		if (ChangeStyleToUnique)
		{
			PlayAnimMontage(ChangeStyleToUnique);
		}
		return;
	}
	if (AttackState == EAttackState::EATS_UniqueAbilityState)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("EATS_UniqueAbilityState"));
		AttackState = EAttackState::EATS_SwordState;
		if (ChangeStyleToBasic)
		{
			PlayAnimMontage(ChangeStyleToBasic);
		}
	}
}


void AHeroes::AttackBySelectedSkill()
{
	if (Target)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SelectEnemy Target Is Valid"));
		IUseSkill* TargetBySkill = Cast<IUseSkill>(Target);
		if (TargetBySkill)
		{
			//		UE_LOG(LogTemp, Warning, TEXT("TargetBySkill Is Valid"));
			//IHitInterface* HitTargetBySkill = Cast<IHitInterface>(Target);
			TargetBySkill->GetDamagedBySkill(this, SkillDamage, SelectedSkill);
		}
		TargetBySkill->HideAimUI();
	}
	Target = nullptr;
}

void AHeroes::HideTargetUI()
{
	if (Target)
	{
		IUseSkill* TargetBySkill = Cast<IUseSkill>(Target);
		if (TargetBySkill)
		{
			TargetBySkill->HideAimUI();
		}
	}
}

void AHeroes::ShowTargetUI()
{
	if (Target)
	{
		IUseSkill* TargetBySkill = Cast<IUseSkill>(Target);
		if (TargetBySkill)
		{
			TargetBySkill->ShowAimUI();
		}
	}
}

UNiagaraComponent* AHeroes::SetTargetSkillParticle(UNiagaraSystem* ParticleSystem)
{
	UNiagaraComponent* SpawnedSystem = nullptr;
	if (Target)
	{

		SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ParticleSystem,
			Target->GetActorLocation()

		);
		if (SpawnedSystem != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AHeroes SetTargetSkillParticle Is Valid"));
		}
	}
	return SpawnedSystem;
}
void AHeroes::GetDamagedBySkill(AActor* Hitter, float Damage, ESkillAttribute SkillAttribute)
{
	Super::GetDamagedBySkill(Hitter, Damage, SkillAttribute);
	UE_LOG(LogTemp, Warning, TEXT("AHeroes GetDamagedBySkill Is Valid"));

	Attributes->ReceiveDamage(Damage);
	CharOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
}


void AHeroes::Shift()
{
	if (ShiftMontage)
	{
		PlayAnimMontage(ShiftMontage);
	}
}

void AHeroes::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AHeroes::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AHeroes::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
}



void AHeroes::HitReactEnd()
{

	ActionState = EActionState::EAS_Unoccupied;
}



void AHeroes::PossessedBy(AController* NewController) // PlayerController
{
	Super::PossessedBy(NewController);

	if (!(NewController->IsA(AAIController::StaticClass())))
	{
		//	UE_LOG(LogTemp, Warning, TEXT("Player Controller :: Possess Called"));
		AIPossessState = EAIPossessState::EPS_PlayerPossess;
		MovementComponent = GetCharacterMovement();

		if (MovementComponent)
		{
			// 최대 이동 속도 설정
			MovementComponent->MaxWalkSpeed = PossessedSpeed;
		}
	}
}

void AHeroes::UnPossessed()
{
	Super::UnPossessed();
	//UE_LOG(LogTemp, Warning, TEXT("UnPossess Called"));

	AIController = GetWorld()->SpawnActor<AAIController>();
	if (AIController)
	{
		AIController->Possess(this);

		AIPossessState = EAIPossessState::EPS_AIPossess;
		MovementComponent = GetCharacterMovement();

		if (MovementComponent)
		{
			// 최대 이동 속도 설정
			MovementComponent->MaxWalkSpeed = ChasePlayerSpeed;
		}

	}

}



void AHeroes::StartAttackTimer()
{
	//	UE_LOG(LogTemp, Warning, TEXT("Heroes : StartAttackTimer called"));
	ActionState = EActionState::EAS_Attacking;
	const float	AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AHeroes::AttackR, AttackTime);

}

void AHeroes::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AHeroes::ChaseTarget()
{
}

void AHeroes::TargetIsDead()
{
	if (Target->ActorHasTag("Dead")) {
		//	UE_LOG(LogTemp, Warning, TEXT("Main : Tick : Enemy Dead"));
		Target = nullptr;
		ActionState = EActionState::EAS_Unoccupied;
		//	TArray<AActor*> Enemies;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), Enemies);
		if (Enemies.Num() > 0)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Main :: Enemies Exist"));
			Target = FindClosestEnemy();
		}
	}
}
void AHeroes::SetHidden()
{
	GetMesh()->SetVisibility(false);
}

void AHeroes::UseSkillHeal()
{
	if (CanAttack())
	{
		//		PlayAttackMontageInOrder();
		//	PlayAttackMontageRandomG();
		PlaySkillMontageRandom();
		ActionState = EActionState::EAS_Attacking;

		if (IsAlive())
		{
			Attributes->Heal(HealAmount);
			CharOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		}
	}
}

