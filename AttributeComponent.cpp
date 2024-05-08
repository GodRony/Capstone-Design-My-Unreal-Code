// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "GameInstance/MyGameInstance.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
} 

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::Heal(float Amount)
{
	if (Health >= MaxHealth) {
		Health = MaxHealth;
		return;
	}
	else {
		Health += Amount;
	}
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetMPPercent()
{
	return MP / MaxMP;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}


bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

void UAttributeComponent::AddSoul(int32 NumOfSouls)
{
	Soul += NumOfSouls;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

bool UAttributeComponent::UseMP(float ComsumeMP)
{
	if (MP - ComsumeMP >= 0)
	{
		MP = MP - ComsumeMP;
		return true;
	}
	return false;
}

bool UAttributeComponent::UseStamina(float ComsumeStamina)
{
	if (Stamina - ComsumeStamina >= 0)
	{
		Stamina = Stamina - ComsumeStamina;
		return true;
	}
	return false;
}

void UAttributeComponent::AddXP(float Amount)
{
	CurrentXP += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Current XP : %f and Current Level : %d"), CurrentXP, CharacterLevel);
	if (CurrentXP >= BaseXP * FMath::Pow(Multiplier, CharacterLevel))
	{
		CharacterLevel++;
		CurrentXP = 0;
		return;
	}

}


