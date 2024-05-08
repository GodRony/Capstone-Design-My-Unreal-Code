// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletPMC.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABulletPMC::ABulletPMC()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
    RootComponent = SphereComponent;
    SphereComponent->SetSphereRadius(3.0f);
    SphereComponent->SetSimulatePhysics(false);
    SphereComponent->SetNotifyRigidBodyCollision(true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->UpdatedComponent = RootComponent; // Assuming your projectile has a root component
    ProjectileMovement->InitialSpeed = 3000.0f; // Set the initial speed
    ProjectileMovement->MaxSpeed = 3000.0f; // Set the maximum speed
    ProjectileMovement->bShouldBounce = false; // Set whether the projectile should bounce off surfaces
    SphereComponent->OnComponentHit.AddDynamic(this, &ABulletPMC::OnHit);
  //  SetLifeSpan(0.5f);
    
}




void ABulletPMC::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
   // UE_LOG(LogTemp, Warning, TEXT("On Hit Called"));
    Destroy();
    if (OtherActor->ActorHasTag("Enemy")) {
        UE_LOG(LogTemp, Warning, TEXT("Enemy On Hit"));
        this->Destroy();

        IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor);
        if (HitInterface) {
            HitInterface->GetHit(GetActorLocation(),10.f,this);
           
        }
    }

}

void ABulletPMC::GetHit(const FVector& ImpactPoint, float Damage, AActor* Hitter)
{
  
}
