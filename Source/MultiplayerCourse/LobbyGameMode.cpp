// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumOfPlayers;
	if(NumOfPlayers >=3)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		bUseSeamlessTravel = true;
		World->ServerTravel(TravelURL);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumOfPlayers;
}
