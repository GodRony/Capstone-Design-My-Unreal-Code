// Fill out your copyright notice in the Description page of Project Settings.


#include "SinbiAnimInstatnce.h"
#include "CharSinbi.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
void USinbiAnimInstatnce::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AnimCharSinbi = Cast<ACharSinbi>(TryGetPawnOwner());
	if (AnimCharSinbi) {
		CharSinbiMovement = AnimCharSinbi->GetCharacterMovement();
	}
}

void USinbiAnimInstatnce::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharSinbiMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharSinbiMovement->Velocity);
		IsFalling = CharSinbiMovement->IsFalling();
		ActionState = AnimCharSinbi->GetActionState();
		DeathPose = AnimCharSinbi->GetDeathPose();
		IsAttacking = AnimCharSinbi->IsAttacking();
		AttackState = AnimCharSinbi->GetAttackState();
	}
}
