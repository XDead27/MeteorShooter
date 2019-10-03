// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = PCapsule;

	PFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Projectile Flipbook"));
	PFlipbook->AttachTo(RootComponent);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	
	PFlipbook->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

