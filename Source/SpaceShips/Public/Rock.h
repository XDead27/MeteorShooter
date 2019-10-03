// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS()
class SPACESHIPS_API ARock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARock();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* PFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trajectories, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Death")
	void OnDeath();
protected:
	UPROPERTY(EditDefaultsOnly, Category = Rock)
	TSubclassOf<class ARock> RockClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateRock();

	float temp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	float size;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* FlipbookSize3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* FlipbookSize2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* FlipbookSize1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* FlipbookExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rendering, meta = (AllowPrivateAccess = "true"))
	float ZOrder;
};
