// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Main.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet/BulletPMC.h"
#include "Soul.h"

AMain::AMain()
{
	PrimaryActorTick.bCanEverTick = true;


	Tags.Add(FName("Main"));

}

void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMain::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMain::OnWeaponOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AMain::OnWeaponEndOverlap);


	InitializeCharOverlay();
}


void AMain::AttackR()
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

void AMain::AttackG()
{
	if (Victim) {
		//Super::AttackG();
//		PlayAnimMontage();

		IAssassination* CharacterVictim = Cast<IAssassination>(Victim);
		if (CharacterVictim)
		{
			CharacterVictim->PlayAssassinated();
		}
		return;
	}


}

void AMain::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{

	if (WeaponBox) {
		UE_LOG(LogTemp, Warning, TEXT("BC::SetWeaponCollisionEnalbed Called"));

		WeaponBox->SetCollisionEnabled(CollisionEnabled);
		IgnoreActors.Empty();
	}
}


void AMain::Die()
{
	Super::Die();
}

void AMain::GetAssassinated(AActor* Victims)
{
	UE_LOG(LogTemp, Warning, TEXT("Main : GetAssassinated Called, Victim is set"));
	Victim = Victims;
}

void AMain::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	UE_LOG(LogTemp, Warning, TEXT("Main OnWeaponOverlap Called"));

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
			HitInterface->GetHit(BoxHit.ImpactPoint, SwordDamage, this);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
		//BoxHit.GetActor()->DecreaseHealth(SwordDamage);
	}

}

void AMain::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}




void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMain::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Damage, Hitter);
	UE_LOG(LogTemp, Warning, TEXT("Main Get Hit"));
}


void AMain::AddSoul(ASoul* Soul)
{
}

bool AMain::IsAttacking()
{
	return false;
}

