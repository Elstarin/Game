// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UCLASS(config=Game)
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

	private:
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

		/** Follow camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

		/** Projectile sphere */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ProjectileSphere;
	public:
		// Constructor for AFPSCharacter
		AGameCharacter(const FObjectInitializer& ObjectInitializer);

		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

		/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;
		
		// Projectile movement component
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
		UProjectileMovementComponent* ProjectileMovement;

		// called when projectile hits something
		UFUNCTION()
		void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
				
		// Gun muzzle's offset from the camera location
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
		FVector MuzzleOffset;

		// inits velocity of the projectile in the shoot direction
		void InitVelocity(const FVector& ShootDirection);
	protected:
		/** Called for forwards/backward input */
		void MoveForward(float Value);

		/** Called for side to side input */
		void MoveRight(float Value);

		/**
		 * Called via input to turn at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void TurnAtRate(float Rate);

		/**
		 * Called via input to turn look up/down at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void LookUpAtRate(float Rate);

		/** Handler for when a touch input begins. */
		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

		/** Handler for when a touch input stops. */
		void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

		/** Handlers for when the left mouse is clicked. */
		void OnLeftMouseButtonDown();
		void OnLeftMouseButtonUp();
		void OnRightMouseButtonDown();
		void OnRightMouseButtonUp();

	protected:
		// APawn interface
		virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
		// End of APawn interface

	public:
		/** Returns CameraBoom subobject **/
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; };
		/** Returns FollowCamera subobject **/
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; };
};
