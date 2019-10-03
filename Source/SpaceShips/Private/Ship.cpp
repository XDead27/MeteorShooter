// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Math.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMathLibrary.inl"
#include "Rock.h"

#define PI 3.14159265

AShip::AShip()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleRadius(16.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(16.0f);


	//Create a background sprite 
	Background_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Background_Sprite"));
	Background_Sprite->SetupAttachment(RootComponent);
	Background_Sprite->SetSimulatePhysics(true);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Background_Sprite);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 640.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Create an arrow object
	Projectile_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile_Arrow"));
	Projectile_Arrow->SetupAttachment(RootComponent);
	

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->AirControl = 0.60f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 300.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;


	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Initialization of variables
	bgmove_coeficient = 1.0f;
	bgoffsetX = 0;
	bgoffsetZ = 0;
	inclin = 0;
	MaxHealth = 100.0f;
	MaxAmmo = 25.0f;
	HealthLeft = MaxHealth;
	AmmoLeft = MaxAmmo;
	AmmoRegen = 2;
}

void AShip::EnemyHit_Implementation()
{
	HealthLeft -= 20;
	UE_LOG(LogTemp, Warning, TEXT("Hit"))
}

void AShip::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? TurningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AShip::UpdateShip()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection <= 0.0f)
		{
			GetSprite()->SetWorldRotation(FRotator(inclin, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			GetSprite()->SetWorldRotation(FRotator(-inclin, 0.0f, 0.0f));
		}
	}

	Background_Sprite->SetRelativeLocation(FVector(bgmove_coeficient*bgoffsetX, -100.0f, bgmove_coeficient*bgoffsetZ));
	Projectile_Arrow->SetWorldRotation(FRotator(90 + inclin, 180.0f, 0.0f));
}

void AShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateAmmo(DeltaSeconds);
	UpdateShip();
}

void AShip::MoveRight(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(cos(inclin*PI / 180)*Value, 0.0f, -sin(inclin*PI / 180)*Value));
	bgoffsetX = GetWorld()->OriginLocation.X - GetSprite()->GetComponentLocation().X;
}

void AShip::MoveUp(float Value)
{
	AddMovementInput(FVector(sin(inclin*PI / 180)*Value, 0.0f, cos(inclin*PI / 180)*Value));
	bgoffsetZ = GetWorld()->OriginLocation.Z - GetSprite()->GetComponentLocation().Z;
}


void AShip::RotateRight(float Value)
{
	inclin += Value;
}

void AShip::Shoot()
{
	if (AmmoLeft >= 1) {
		FActorSpawnParameters SpwnPrmt;
		SpwnPrmt.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpwnPrmt.Owner = this;
		SpwnPrmt.Instigator = Instigator;

		Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Projectile_Arrow->GetComponentTransform(), SpwnPrmt);

		AmmoUsed();
		AmmoLeft--;
	}
}

void AShip::UpdateAmmo(float DeltaSeconds)
{
	AmmoLeft = fmin(AmmoLeft + DeltaSeconds*AmmoRegen, MaxAmmo);
}

void AShip::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AShip::MoveUp);
	PlayerInputComponent->BindAxis("RotateRight", this, &AShip::RotateRight);
	//PlayerInputComponent->BindAxis("Shot", this, &AShip::Shoot);

	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AShip::Shoot);
}