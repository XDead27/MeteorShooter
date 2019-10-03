// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Ship.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHIPS_API AShip : public APaperCharacter
{
	GENERATED_BODY()
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* Background_Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Projectile_Arrow;

	virtual void Tick(float DeltaSeconds) override;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* TurningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ProjectileFlipbook;*/

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);
	void MoveUp(float Value);
	void RotateRight(float Value);
	
	void Shoot();
	void UpdateAmmo(float);

	void UpdateShip();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AShip();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UPaperSpriteComponent* GetBackgroundSprite() const { return Background_Sprite; }

	FORCEINLINE float GetHelathLeft() const { return HealthLeft; }
	FORCEINLINE float GetAmmoLeft() const { return AmmoLeft; }

	UFUNCTION(BlueprintImplementableEvent, Category = "AmmoSystem")
	void AmmoUsed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HealthSystem")
	void EnemyHit();

private:
	float bgoffsetX, bgoffsetZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Background, meta = (AllowPrivateAccess = "true"))
	float bgmove_coeficient;

	float inclin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ship_Stats, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ship_Stats, meta = (AllowPrivateAccess = "true"))
	float MaxAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ship_Stats, meta = (AllowPrivateAccess = "true"))
	float HealthLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ship_Stats, meta = (AllowPrivateAccess = "true"))
	float AmmoLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ship_Stats, meta = (AllowPrivateAccess = "true"))
	float AmmoRegen;

	AProjectile* Proj;
};
