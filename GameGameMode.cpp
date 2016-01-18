// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Game.h"
#include "GameGameMode.h"
#include "GameCharacter.h"
#include "MainHUD.h"

AGameGameMode::AGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	HUDClass = AMainHUD::StaticClass();
}

void AGameGameMode::StartPlay(){
  Super::StartPlay();

  StartMatch();
	
	UWorld* const World = GetWorld();
	UtilitySystem::profileCode(World);
}

void AGameGameMode::Tick(float DeltaSeconds)
{
	// UE_LOG(Main, Warning, TEXT("%f"), TimerSystem::GetCycles());

	// AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}
