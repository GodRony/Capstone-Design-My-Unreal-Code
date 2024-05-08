// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
	PlayDeathMontage();
}

void ABaseCharacter::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{

	if (IsAlive() && Hitter) {
		UE_LOG(LogTemp, Warning, TEXT("BC1 :  GetHit Called"));

		HitEffect(Hitter, ImpactPoint);
	}
	else {
		HitEffect(Hitter, ImpactPoint);
		Die();
	}
}

void ABaseCharacter::HitEffect(AActor* Hitter, const FVector& ImpactPoint)
{
	DirectionalHitReact(Hitter->GetActorLocation());
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	//UE_LOG(LogTemp, Warning, TEXT("BC3 : PlayHitReactMontage GetHit Called"));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	//	UE_LOG(LogTemp, Warning, TEXT("BC2 : DirectionalHitReact GetHit Called"));

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f; // 언리얼은 왼손법칙을 따름 
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
}




void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage) {
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::GetDamagedBySkill(class AActor* Hitter, float Damage, ESkillAttribute SkillAttribute)
{
	if (DamagedBySkillMontage)
	{
		//DamageWidget->ShowDamageUI(Damage);
		//	GetHit(GetActorLocation(),Damage,nullptr);

		if (SkillAttribute == Weak)
		{
			DamageAmount = Damage + Damage * 0.8f;
			this->PlayAnimMontage(DamagedBySkillMontage);
		}
		else if (SkillAttribute == Block)
		{
			DamageAmount = Damage - Damage * 0.3;
			this->PlayAnimMontage(DamagedBySkillMontage);
		}
		else if (SkillAttribute != Weak && SkillAttribute != Block)
		{
			DamageAmount = Damage;
			this->PlayAnimMontage(DamagedBySkillMontage);
		}
		UDamageUI* DamageWidget = CreateWidget<UDamageUI>(GetWorld(), DamageWidgetClass);
		if (DamageWidget)
		{
			DamageWidget->ShowDamageUI(DamageAmount);
			DamageWidget->AddToViewport();
		}
	}

}



int32 ABaseCharacter::PlayAttackMontageRandomBasicR()
{
	return PlayRandomMontageSection(AttackMontageBasicR, AttackMontageSectionsBasicR);

}

int32 ABaseCharacter::PlayAttackMontageRandomUniqueR()
{
	return PlayRandomMontageSection(AttackMontageUniqueR, AttackMontageSectionsUniqueR);
}

int32 ABaseCharacter::PlaySkillMontageRandom()
{
	return PlayRandomMontageSection(SkillMontage, SkillMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		//	UE_LOG(LogTemp, Warning, TEXT("BC::SetWeaponCollisionEnalbed Called"));
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();

	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}
