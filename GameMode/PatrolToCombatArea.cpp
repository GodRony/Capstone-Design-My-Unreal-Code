// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PatrolToCombatArea.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameInstance/MyGameInstance.h"
#include "Heroes.h"
// Sets default values
APatrolToCombatArea::APatrolToCombatArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SetActorTickEnabled(false);


    // Setup the root component
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    // Setup the collision volume
     BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    BoxComponent->SetupAttachment(GetRootComponent());
    if (BoxComponent) {
        BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APatrolToCombatArea::OnOverlapBegin);
    }
}

// Called when the game starts or when spawned
void APatrolToCombatArea::BeginPlay()
{
	Super::BeginPlay();

}

void APatrolToCombatArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("APatrolToCombatArea OnOverlapBegin Called"));
  
   if (AHeroes* OtherHeroes = Cast<AHeroes>(OtherActor)) {
        OtherHeroes->SetCombatGameState();
        OtherHeroes->FindEnemy();
    }
 
}



