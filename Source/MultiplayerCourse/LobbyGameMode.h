// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerCourseGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERCOURSE_API ALobbyGameMode : public AMultiplayerCourseGameMode
{
	GENERATED_BODY()

public:
		void PostLogin(APlayerController* NewPlayer) override;	
		void Logout(AController* Exiting) override;

private:
	uint32 NumOfPlayers = 0;
	FString TravelURL = "/Game/Maps/M_Playground?listen";

	void TravelToMap();
};
