// Fill out your copyright notice in the Description page of Project Settings.


#include "CharSinbi.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttributeComponent.h"
#include "CharOverlay.h"
#include "GameFramework/Controller.h"
#include "Soul.h"
// Sets default values
ACharSinbi::ACharSinbi()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Sinbi"));

}



void ACharSinbi::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{
	//Super::GetHit(ImpactPoint, Damage, Hitter);
	//UE_LOG(LogTemp, Warning, TEXT("Shinbi GetHit Called"));
	if (IsAlive() && Hitter) {
		//UE_LOG(LogTemp, Warning, TEXT("BC1 :  GetHit Called"));

		HitEffect(Hitter, ImpactPoint);
	}
	else {
		HitEffect(Hitter, ImpactPoint);
		Die();

	}
	ClearAttackTimer();
	if (IsAlive())
	{
		Attributes->ReceiveDamage(Damage);
		CharOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		ActionState = EActionState::EAS_HitReaction;
	}

}

// Called when the game starts or when spawned
void ACharSinbi::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ACharSinbi::OnWeaponOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &ACharSinbi::OnWeaponEndOverlap);
	Tags.Add(FName("CharSinbi"));
	InitializeCharOverlay();
}


// Called every frame
void ACharSinbi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharSinbi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharSinbi::AddSoul(ASoul* Soul)
{
	Super::AddSoul(Soul);

}

void ACharSinbi::Die()
{
	Super::Die();

}

void ACharSinbi::Shift()
{
	Super::Shift();


	//UE_LOG(LogTemp, Warning, TEXT("Shift Called"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FVector ForwardVector = GetActorForwardVector(); // 캐릭터가 바라보는 방향의 벡터
	FVector NewLocation = GetActorLocation() + ForwardVector * DashDistance; // 현재 위치에서 바라보는 방향으로 500만큼 이동

	SetActorLocation(NewLocation);
}

void ACharSinbi::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = WeaponTraceStart->GetComponentLocation();
	const FVector End = WeaponTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		WeaponTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint, SwordDamage,this);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
		//BoxHit.GetActor()->DecreaseHealth(SwordDamage);
	}
	
}

void ACharSinbi::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



void ACharSinbi::AttackR()
{
//	Super::AttackR();
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
			ShootingBullet();
			//ActionState = EActionState::EAS_Attacking;
		}
	}

}

void ACharSinbi::AttackG()
{
	Super::AttackG();
}


void ACharSinbi::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{	
	if (WeaponBox) {
		WeaponBox->SetCollisionEnabled(CollisionEnabled);
		IgnoreActors.Empty();
	}

}


void ACharSinbi::ShootingBullet()
{
	if (BulletClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("BulletClass is nullptr"));
		return;
	}
	FVector ForwardVector = GetActorForwardVector();
	float SpawnDistance = 40.f;
	float ZOffset = 30.f;
	FVector SpawnLocation = GetActorLocation() + (ForwardVector * SpawnDistance) + FVector(0.0f, 0.0f, ZOffset);

	ABulletPMC* NewBullet = GetWorld()->SpawnActor<ABulletPMC>(BulletClass, SpawnLocation, GetActorRotation());

	// 생성된 총알에 수동으로 SetLifeSpan 설정
	if (NewBullet != nullptr)
	{
		//NewBullet->SetLifeSpan(0.3f);
		UE_LOG(LogTemp, Warning, TEXT("SetLifeSpan called for the bullet"));
	}
}

void ACharSinbi::StartShootingProjectile()
{
	IsShootingProjectile = true;
}

void ACharSinbi::EndShootingProjectile()
{
	IsShootingProjectile = false;

}
