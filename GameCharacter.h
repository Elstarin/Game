// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Magic.h"
#include "GameCharacter.generated.h"

UCLASS() // config=Game
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

	private:
		// APlayerController* ThePlayerPtr;
		// AGameCharacter& ThePlayerPtr;
		
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
		static APlayerController* ThePlayerController;
		static APlayerController* GetPlayer();
		// Constructor for AFPSCharacter
		AGameCharacter(const FObjectInitializer& ObjectInitializer);
		
		virtual void PostInitializeComponents() override;
		virtual void Tick(float DeltaTime) override;

		/** In first person mode? */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Gameplay)
		bool bFirstPersonMode;

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
		
		UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		UFUNCTION()
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
				
		// Gun muzzle's offset from the camera location
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
		FVector MuzzleOffset;
		
		// // Spell class to spawn
		// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Magic)
		// AMagic* magic;
		
		// Spell class to spawn
		// UPROPERTY(EditDefaultsOnly, Category=Magic)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Magic)
		TSubclassOf<class AMagic> MagicClass;
		
		// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Magic)
		// AMagic* MagicClass;
		
		static Frame* frame;
		void SetupFrame();
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
		
		/*--------------------------------------------------------------------------
				Camera zoom stuff
		--------------------------------------------------------------------------*/
		//Input variables
		FVector2D MovementInput;
		FVector2D CameraInput;
		float ZoomFactor;
		
		void ZoomIn();
		void ZoomOut();
		

	protected:
		// APawn interface
		virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
		// End of APawn interface

	public:
		// FORCEINLINE APlayerController* GetPlayerChar() const { return GetWorld()->GetOwningPlayerController(); };
		/** Returns FollowCamera subobject **/
		FVector GetPlayerLocation();
		/** Returns CameraBoom subobject **/
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; };
		/** Returns FollowCamera subobject **/
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; };
};
