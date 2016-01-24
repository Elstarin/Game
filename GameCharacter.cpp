// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Game.h"
#include "GameCharacter.h"
#include "Magic.h"

/*------------------------------------------------------------------------------
		Possible talents/perks
		
		~ Arrows a slight trail/trace
		~ Passively increased health/resource regen
		~ Melee weapons can sweep through all targets?
------------------------------------------------------------------------------*/
Frame* AGameCharacter::frame;

/*------------------------------------------------------------------------------
		Main constructor
------------------------------------------------------------------------------*/
AGameCharacter::AGameCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AGameCharacter::OnOverlapEnd);

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
	ZoomFactor = 0.2;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	/*----------------------------------------------------------------------------
			Create a follow camera
	----------------------------------------------------------------------------*/
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	MagicClass = AMagic::StaticClass();
	
	MuzzleOffset = FVector(80.0f, 0.0f, -30.0f);
	
	SetupFrame();
}

void AGameCharacter::SetupFrame()
{
	static bool hasRun = false;
  if (hasRun) return;
  hasRun = true;
	
	frame = Frame::CreateFrame("Actor", "PlayerFrame", "BACKGROUND", 0);
	frame->Hide();
	
	// frame->SetEvent(EventEnum::MOUSE_LEFT_CLICK_DOWN, [](const auto* obj)
	// {
	// 	// print(TimerSystem::GetTime(), "Left click down 1", obj->event);
	// });
	//
	// frame->SetEvent(EventEnum::MOUSE_LEFT_CLICK_DOWN, [](const auto* obj)
	// {
	// 	// print(TimerSystem::GetTime(), "Left click down 2", obj->event);
	// });
	
	// frame->SetEvent(EventEnum::MOUSE_LEFT_CLICK_UP, []()
	// {
	// 	// print(TimerSystem::GetTime(), "Left click up");
	// });
}

void AGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// print("PostInitialize");
	
	// MagicClass = TSubclassOf<class AMagic>;
	
	// // UPROPERTY(EditDefaultsOnly, Category=Magic)
	// TSubclassOf<class AMagic> MagicClass;
	
	// AMagic* MagicClass;
}
/*------------------------------------------------------------------------------
		Player input constructor
------------------------------------------------------------------------------*/
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
	
	// Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AGameCharacter::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AGameCharacter::ZoomOut);
};
/*------------------------------------------------------------------------------
		Look/turn input
------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------
		Movement input
------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------
		Mouse input
------------------------------------------------------------------------------*/
void AGameCharacter::OnLeftMouseButtonDown()
{
	// frame->FireToFrame(EventEnum::MOUSE_LEFT_CLICK_DOWN);
	// print(Frame::GetNumFrames());
	print(TimerSystem::GetTime(), "OnLeftMouseButtonDown");
	
	// Make sure it has been initialized
	if (MagicClass != NULL)
	{
		static FVector location;
		static FRotator rotation;
		
		// Get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		
		// If in first person mode, fire projectile based on camera
		if (bFirstPersonMode)
		{
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
			FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRot;
			MuzzleRotation.Pitch += 10.0f; // skew the aim upwards a bit
			
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				// spawn the projectile at the muzzle
				AMagic* const Spell = World->SpawnActor<AMagic>(MagicClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		
				if (Spell)
				{
					// find launch direction
					FVector const LaunchDir = MuzzleRotation.Vector();
					Spell->InitVelocity(LaunchDir);
				}
			}
		}
		else
		{
			location = GetActorLocation() + GetActorForwardVector();
			rotation = GetActorRotation();
			rotation.Pitch = (CameraRot.Pitch + 10.0f);
			// FVector forward = GetActorForwardVector();
		}
		
		FVector const FinalLocation = location + FTransform(rotation).TransformVector(MuzzleOffset);
		
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// SpawnParams.bNoCollisionFail = true;
			// spawn the projectile at the muzzle
			AMagic* const Spell = World->SpawnActor<AMagic>(MagicClass, FinalLocation, rotation, SpawnParams);
	
			if (Spell)
			{
				// FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
				// FRotator MuzzleRotation = CameraRot;
				
				// find launch direction
				FVector const LaunchDir = rotation.Vector();
				Spell->InitVelocity(LaunchDir);
			}
		}
		
		// FVector location = GetActorLocation();
		// location.Y += 120.0f;
		// location += GetActorForwardVector();
		// FVector forward = GetActorForwardVector();
		
		// CameraRot.Pitch += 20.0f;
		
		// print(location - CameraLoc);
		
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
		// FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		// FRotator MuzzleRotation = CameraRot;
		// MuzzleRotation.Pitch += 10.0f; // skew the aim upwards a bit
		
		// FVector V = GetVelocity();
		// FRotator R = GetActorRotation();
		// V = R.UnrotateVector(V);
	}
}

void AGameCharacter::OnLeftMouseButtonUp()
{
	// print("Left mouse UP");
}

void AGameCharacter::OnRightMouseButtonDown()
{
	// print("Right mouse DOWN");
}

void AGameCharacter::OnRightMouseButtonUp()
{
	// print("Right mouse UP");
}
/*------------------------------------------------------------------------------
		Zoom in/out
------------------------------------------------------------------------------*/
void AGameCharacter::ZoomIn()
{
	ZoomFactor += 0.5f;
	
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, -5.0f, 4.1f);
	
	print("Zoom:", ZoomFactor);
	
	//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
	// FollowCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
	CameraBoom->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
}

void AGameCharacter::ZoomOut()
{
	ZoomFactor -= 0.5f;
	
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, -5.0f, 4.1f);
	
	print("Zoom:", ZoomFactor);
	
	//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
	// FollowCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
	CameraBoom->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
}
/*------------------------------------------------------------------------------
		Tick
------------------------------------------------------------------------------*/
void AGameCharacter::Tick(float DeltaTime)
{
	// //Rotate our actor's yaw, which will turn our camera because we're attached to it
  // FRotator NewRotation = GetActorRotation();
  // NewRotation.Yaw += CameraInput.X;
  // SetActorRotation(NewRotation);
	
	FRotator ActorRotation = GetActorRotation();
	
	//Rotate our camera's pitch, but limit it so we're always looking downward
  FRotator NewRotation = CameraBoom->GetComponentRotation();
  NewRotation.Pitch = ActorRotation.Pitch;
  // NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
	SetActorRotation(NewRotation);
  // CameraBoom->SetWorldRotation(NewRotation);
}
/*------------------------------------------------------------------------------
		Events
------------------------------------------------------------------------------*/
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

void AGameCharacter::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	print("Overlap begin");
  if (OtherActor && (OtherActor != this) && OtherComp)
  {
    // PointLight->SetVisibility(true);
  }
};

void AGameCharacter::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	print("Overlap end");
  if (OtherActor && (OtherActor != this) && OtherComp)
  {
		// PointLight->SetVisibility(false);
  }
};
