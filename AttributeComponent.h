// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WINTERPROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UAttributeComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);
	void Heal(float Amount);
	float GetHealthPercent();
	float GetMPPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSoul(int32 NumOfSouls);
	void AddGold(int32 AmountOfGold);

	UFUNCTION(BlueprintCallable)
	bool UseMP(float ComsumeMP);
	UFUNCTION(BlueprintCallable)
	bool UseStamina(float ComsumeStamina);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float MaxMP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float MP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	float Stamina;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Attributes")
	int32 CharacterLevel = 1;
	float CurrentXP = 0;
	float BaseXP = 100.0f;
	float Multiplier = 1.2f;
	void AddXP(float Amount);

private:
//	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
//		float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Gold;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Soul;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float XP;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE float GetXP() const { return XP; }
};
