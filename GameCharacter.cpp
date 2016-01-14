// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Game.h"
#include "GameCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AGameCharacter

AGameCharacter::AGameCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	auto PlayerFrame = Frame::CreateFrame("Actor", "PlayerFrame", "BACKGROUND", 0);
	PlayerFrame->Hide();
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	/*----------------------------------------------------------------------------
			Create a camera boom (pulls in towards the player if there is a collision)
	----------------------------------------------------------------------------*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	/*----------------------------------------------------------------------------
			Create a follow camera
	----------------------------------------------------------------------------*/
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	/*----------------------------------------------------------------------------
			Create the projectile object
	----------------------------------------------------------------------------*/
	// // Use a sphere as a simple collision representation
	// ProjectileSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("ProjectileSphere"));
	// // ProjectileSphere->BodyInstance.SetCollisionProfileName("Projectile");
	// // ProjectileSphere->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	// ProjectileSphere->InitSphereRadius(15.0f);
	// // RootComponent = ProjectileSphere;
	//
	// // Create and position a mesh component so we can see where our sphere is
	// UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereVisual"));
	// SphereVisual->AttachTo(ProjectileSphere);
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	//
	// if (SphereVisualAsset.Succeeded())
	// {
	// 	SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	// 	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
	// 	// SphereVisual->SetWorldScale3D(FVector(0.8f));
	// }
	//
	// // Use a ProjectileMovementComponent to govern this projectile's movement
	// ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	// ProjectileMovement->UpdatedComponent = ProjectileSphere;
	// ProjectileMovement->InitialSpeed = 3000.f;
	// ProjectileMovement->MaxSpeed = 3000.f;
	// ProjectileMovement->bRotationFollowsVelocity = true;
	// ProjectileMovement->bShouldBounce = true;
	// ProjectileMovement->Bounciness = 0.5f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AGameCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AGameCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AGameCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AGameCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AGameCharacter::TouchStopped);
	
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AGameCharacter::OnLeftMouseButtonDown);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AGameCharacter::OnLeftMouseButtonUp);
	
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AGameCharacter::OnRightMouseButtonDown);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AGameCharacter::OnRightMouseButtonUp);
};


void AGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGameCharacter::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AGameCharacter::OnLeftMouseButtonDown()
{
	print("Left mouse DOWN");
	
	// Get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);
	// // MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
	// FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
	
	UWorld* const World = GetWorld();
	
	// if (World)
	// {
	// 	FActorSpawnParameters SpawnParams;
	// 	SpawnParams.Owner = this;
	// 	SpawnParams.Instigator = Instigator;
	//
	// 	// Spawn the projectile at the muzzle
	// 	ProjectileSphere* const Projectile = World->SpawnActor<ProjectileSphere>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	// 	if (Projectile)
	// 	{
	// 		// find launch direction
	// 		FVector const LaunchDir = MuzzleRotation.Vector();
	// 		Projectile->InitVelocity(LaunchDir);
	// 	}
	// }
	
	// // try and fire a projectile
	// if (ProjectileClass != NULL)
	// {
	// 	// Get the camera transform
	// 	FVector CameraLoc;
	// 	FRotator CameraRot;
	// 	GetActorEyesViewPoint(CameraLoc, CameraRot);
	// 	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
	// 	FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
	// 	FRotator MuzzleRotation = CameraRot;
	// 	MuzzleRotation.Pitch += 10.0f; // skew the aim upwards a bit
	// 	UWorld* const World = GetWorld();
	//
	// 	if (World)
	// 	{
	// 		FActorSpawnParameters SpawnParams;
	// 		SpawnParams.Owner = this;
	// 		SpawnParams.Instigator = Instigator;
	// 		// spawn the projectile at the muzzle
	// 		AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	// 		if (Projectile)
	// 		{
	// 			// find launch direction
	// 			FVector const LaunchDir = MuzzleRotation.Vector();
	// 			Projectile->InitVelocity(LaunchDir);
	// 		}
	// 	}
	// }
}

void AGameCharacter::OnLeftMouseButtonUp()
{
	print("Left mouse UP");
}

void AGameCharacter::OnRightMouseButtonDown()
{
	print("Right mouse DOWN");
}

void AGameCharacter::OnRightMouseButtonUp()
{
	print("Right mouse UP");
}

void AGameCharacter::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// // If it exists and isn't itself
	// if (OtherActor && (OtherActor != this) && (OtherComp != nullptr)){
	// 	// Trigger explosion on impact
	// 	if (ParticleEffectCompImpact && ParticleEffectCompImpact->Template)
	// 	{
	// 		ParticleEffectCompImpact->ToggleActive();
	// 	}
	//
	// 	// If it has physics (AKA can we push it back?)
	// 	if (OtherComp->IsSimulatingPhysics())
	// 	{
	// 		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
	// 	}
	// }
}
