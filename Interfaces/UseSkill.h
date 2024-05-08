// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterType.h"
#include "UseSkill.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUseSkill : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINTERPROJECT_API IUseSkill
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
//	UFUNCTION(BlueprintCallable)
//	virtual void GetDamagedBySkill(class AActor* SkillTarget);

	virtual void GetDamagedBySkill(class AActor* Hitter,float Damage, ESkillAttribute SkillAttribute);
	virtual void ShowAimUI();
	virtual void HideAimUI();
	

};
