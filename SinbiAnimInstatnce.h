// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterType.h"
#include "SinbiAnimInstatnce.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API USinbiAnimInstatnce : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, Category= Movement)
	class ACharSinbi* AnimCharSinbi;
	UPROPERTY(BlueprintReadWrite,Category = Movement)
	class UCharacterMovementComponent* CharSinbiMovement;
	UPROPERTY(BlueprintReadWrite,Category = Movement)
	float GroundSpeed;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool IsFalling;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
		bool IsAttacking;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	EActionState ActionState;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	EAttackState AttackState;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
		TEnumAsByte<EDeathPose> DeathPose;
};
