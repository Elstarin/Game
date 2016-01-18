// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Magic.h"

// Sets default values
AMagic::AMagic()
{
	PrimaryActorTick.bCanEverTick = true;
}

AMagic::~AMagic()
{
}

AMagic::AMagic(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Projectile sphere /////////////////////////////////////////////////////////
	
	// Use a sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMagic::OnHit);
	RootComponent = CollisionComp;
	
	OnDestroyed.AddDynamic(this, &AMagic::Destroyed);
	
	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.5f;
	
	// Die after 10 seconds by default
	InitialLifeSpan = 5.0f;
	//////////////////////////////////////////////////////////////////////////////
	
	// Particle system ///////////////////////////////////////////////////////////
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleObject(TEXT("ParticleSystem'/Game/Effects/Particles/Line_P.Line_P'"));
	
	// ParticleEffect = ObjectInitializer.CreateDefaultSubobject<UParticleSystem>(this, TEXT("ParticleObject"));
	
	// if (ParticleObject.Object != NULL)
	// {
	// 	ParticleEffect = ParticleObject.Object;
	// 	// ParticleEffect->bAutoActivate = false;
	// 	// ParticleEffect->SetHiddenInGame(false);
	//
	// 	UGameplayStatics::SpawnEmitterAttached(
	// 		ParticleEffect, // Particle system
	// 		CollisionComp, // Mesh to attach to
	// 		FName(), // Socket name
	// 		FVector(0, 0, 0), // Location relative to socket
	// 		FRotator(0, 0, 0), // Rotation
	// 		EAttachLocation::KeepRelativeOffset,
	// 		true // Will be deleted automatically
	// 	);
	// }
	
	// Create a particle system that we can activate or deactivate
	ParticleEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	ParticleEffectComp->AttachTo(SphereVisual);
	ParticleEffectComp->bAutoActivate = false;
	ParticleEffectComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	// ParticleEffectComp->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	
	if (ParticleAsset.Succeeded())
	{
		ParticleEffectComp->SetTemplate(ParticleAsset.Object);
	}
	
	// Create a particle system that we can activate or deactivate
	ParticleEffectCompImpact = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticles"));
	ParticleEffectCompImpact->AttachTo(SphereVisual);
	ParticleEffectCompImpact->bAutoActivate = false;
	ParticleEffectCompImpact->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAssetImpact(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	
	if (ParticleAssetImpact.Succeeded())
	{
		ParticleEffectCompImpact->SetTemplate(ParticleAssetImpact.Object);
		// ParticleEffectCompImpact->SetWorldScale3D(FVector(15.5f));
		// ParticleEffectCompImpact->SetRelativeScale3D(FVector(2.0f));
	}
	
	// if (ParticleObject.Succeeded())
	// {
	// 	UGameplayStatics::SpawnEmitterAttached(
	// 		ParticleEffect, // Particle system
	// 		CollisionComp, // Mesh to attach to
	// 		FName(), // Socket name
	// 		FVector(0, 0, 64), // Location relative to socket
	// 		FRotator(0, 0, 0), // Rotation
	// 		EAttachLocation::KeepRelativeOffset,
	// 		true // Will be deleted automatically
	// 	);
	// }
	
	// if (ParticleObject.Succeeded())
	// {
	// 	ParticleEffect->Template = ParticleObject.Object;
	// }
	
	// ParticleEffect->AttachTo(Cast<USceneComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass())));
	// ParticleEffect->AttachTo(CollisionComp);
	
	// ParticleEffect->ActivateSystem();
	// ParticleEffect->DeactivateSystem();
	
	//effect
	// UGameplayStatics::SpawnEmitterAttached(
	// 	ThePS, //UParticleSystem*
	// 	CurrentPower,
	// 	TipSocketName,
	// 	FVector(-10,0,0),
	// 	FRotator(0,90,0),
	// 	EAttachLocation::KeepRelativeOffset,
	// 	true
	// );
	
	// UGameplayStatics::SpawnEmitterAttached(
	// 	ThePS, // Particle system
	// 	Mesh, // Mesh to attach to
	// 	FName("Head"), // Socket name
	// 	FVector(0,0,64), // Location relative to socket
	// 	FRotator(0,0,0), // Rotation
	// 	EAttachLocation::KeepRelativeOffset,
	// 	true // Will be deleted automatically
	// );
	
	//////////////////////////////////////////////////////////////////////////////
	
	// UParticleSystemComponent* ParticleTemp;
	// UParticleSystemComponent
	
	// UParticleSystemComponent* ParticleTemp;
	// ParticleTemp = UGameplayStatics::SpawnEmitterAttached(ParticleTemplates[0], RootComponent, NAME_None, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
	// ParticleSystems.Add(ParticleTemp);
	
	fSpeed = 3000.f;
	fBounceSpeedLoss = 0.7f;
}

void AMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMagic::BeginPlay()
{
	Super::BeginPlay();
	ParticleToggle();
	
	frame = Frame::CreateFrame("Actor", "MagicFrame", "BACKGROUND", 0);
	frame->Hide();
	
	// auto lambda = [&](auto f){
	// 	print("Lambda", GetActorLocation());
	// 	// GetActorLocation();
	// };
	
	frame->OnUpdate([&](auto f)
	{
		// print("Lambda", GetActorLocation());
		// GetActorLocation();
	});
}

void AMagic::Destroyed()
{
	print("Destroyed!");
	
	frame->DeleteFrame();
	
	// delete frame;
}

void AMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FColor LineColor = bReflected ? FColor::Red : FColor::Green;
	
  DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + MyVelocity * DeltaTime, LineColor, false, 2.f, 0, 3.f);
 
  // SetActorLocation(GetActorLocation() + MyVelocity * DeltaTime, true);
}

void AMagic::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		MyVelocity = GetActorForwardVector() * fSpeed;

		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AMagic::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If it exists and isn't itself
	if (OtherActor && (OtherActor != this) && (OtherComp != nullptr))
	{
		// Trigger explosion on impact
		if (ParticleEffectCompImpact && ParticleEffectCompImpact->Template)
		{
			ParticleEffectCompImpact->ToggleActive();
		}
		
		// If it has physics (AKA can we push it back?)
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			// OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		}
		else
		{
			// Reflect the projectile because we hit a non-physics object
			FVector ReflectedVelocity = fBounceSpeedLoss * (-2 * FVector::DotProduct(MyVelocity, Hit.Normal) * Hit.Normal + MyVelocity);
			
			MyVelocity = ReflectedVelocity;
			ReflectedVelocity.Normalize();
			SetActorRotation(ReflectedVelocity.Rotation());
			
			bReflected = true;
		}
		
		
		// Destroy();
	}
}

void AMagic::ParticleToggle()
{
  if (ParticleEffectComp && ParticleEffectComp->Template)
  {
    ParticleEffectComp->ToggleActive();
  }
}
