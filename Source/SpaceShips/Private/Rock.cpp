// Fill out your copyright notice in the Description page of Project Settings.

#include "Rock.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "Ship.h"
#include "Math.h"
#include "Projectile.h"

// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collsion Sphere"));
	RootComponent = CCapsule;
	CCapsule->bGenerateOverlapEvents = true;
	CCapsule->SetCapsuleRadius(8.0f);
	CCapsule->SetCapsuleHalfHeight(8.0f);
	CCapsule->OnComponentBeginOverlap.AddDynamic(this, &ARock::OnOverlapBegin);
	CCapsule->SetEnableGravity(false);

	PFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Rock Flipbook"));
	PFlipbook->SetupAttachment(RootComponent);
	PFlipbook->bGenerateOverlapEvents = false;
	PFlipbook->SetEnableGravity(false);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	Projectile->InitialSpeed = 50;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Death Trajectory Arrow"));
	Arrow->SetupAttachment(RootComponent);

	PFlipbook->SetIsReplicated(true);

	
	size = 3;

	//The order in which things overlap
	ZOrder = 1;

	temp = 0;
}

void ARock::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor is the actor that triggered the event. Check that is not ourself 
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) {
		return;
	}
	else if (OtherActor->IsA(ATriggerVolume::StaticClass())) {
		//UE_LOG(LogTemp, Warning, TEXT("Triggervolume"));

		FVector newloc(0.0f, ZOrder, 0.0f);
		newloc.X = CCapsule->GetComponentLocation().X > 0 ? -(abs(CCapsule->GetComponentLocation().X) - 1) : abs(CCapsule->GetComponentLocation().X) - 1;
		newloc.Z = CCapsule->GetComponentLocation().Z > 0 ? -(abs(CCapsule->GetComponentLocation().Z) - 1) : abs(CCapsule->GetComponentLocation().Z) - 1;

		CCapsule->SetWorldLocation(newloc);
	}
	else if (OtherActor->IsA(AProjectile::StaticClass())) {
		//UE_LOG(LogTemp, Warning, TEXT("Should be dead"))
	}
}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Updates the flipbook of the rock depending on the size
void ARock::UpdateRock()
{
	if (size == 1) {
		PFlipbook->SetFlipbook(FlipbookSize1);
		CCapsule->SetCapsuleHalfHeight(8.0f);
		CCapsule->SetCapsuleRadius(8.0f);
	}
	else if (size == 2) {
		PFlipbook->SetFlipbook(FlipbookSize2);
		CCapsule->SetCapsuleHalfHeight(16.0f);
		CCapsule->SetCapsuleRadius(16.0f);
	}else if(size == 3){
		PFlipbook->SetFlipbook(FlipbookSize3);
		CCapsule->SetCapsuleHalfHeight(32.0f);
		CCapsule->SetCapsuleRadius(32.0f);
	}
	else {
		PFlipbook->SetFlipbook(FlipbookExplosion);
		temp += GetWorld()->GetDeltaSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f"), fmin(temp + GetWorld()->GetDeltaSeconds(), 0.5))
		if(fmin(temp + GetWorld()->GetDeltaSeconds(), 0.5) == 0.5)
			Destroy();
	}
}

void ARock::OnDeath_Implementation()
{
	if (size > 1) {
		float fractureangle = 30;

		FActorSpawnParameters SpwnPrmt;
		SpwnPrmt.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwnPrmt.Owner = this;

		FRotator firstspwn = GetActorRotation();
		firstspwn.Pitch = GetActorRotation().Pitch + fractureangle;

		FRotator secondspwn = GetActorRotation();
		secondspwn.Pitch = GetActorRotation().Pitch - fractureangle;

		ARock* alfa, *beta;



		alfa = GetWorld()->SpawnActor<ARock>(RockClass, Arrow->GetComponentLocation(), firstspwn, SpwnPrmt);
		beta = GetWorld()->SpawnActor<ARock>(RockClass, Arrow->GetComponentLocation(), secondspwn, SpwnPrmt);

		if (alfa != nullptr && beta != nullptr) {
			alfa->size = this->size - 1;
			beta->size = this->size - 1;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%f"), GetActorRotation().Pitch);

		Destroy();
	}
	else if(size == 1){
		size--;
		CCapsule->bGenerateOverlapEvents = false;
	}
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRock();
}

