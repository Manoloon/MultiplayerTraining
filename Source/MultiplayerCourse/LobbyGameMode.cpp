// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzleGameInstance.h"
#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumOfPlayers;
	// si hay 3 o mas jugadores en el lobby, arrancamos! 
	if(NumOfPlayers >=3)
	{
		FTimerHandle StartTravelTH;
		GetWorldTimerManager().SetTimer(StartTravelTH, this, &ALobbyGameMode::TravelToMap, 10.0f,false,1.0f);				
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumOfPlayers;
}

void ALobbyGameMode::TravelToMap()
{
	auto GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->StartSession();
	}
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	World->ServerTravel(TravelURL);
}
