// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenuInterface.h"
#include "PuzzleGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERCOURSE_API UPuzzleGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()

public:

	UPuzzleGameInstance(const FObjectInitializer& ObjectInitializer);

	
	virtual void Init() override;

	UFUNCTION()
		virtual void Host(FString newServerName) override;
	UFUNCTION()
		virtual void Join(uint32 newIndex) override;
	UFUNCTION()
		virtual void LoadMainMenu() override;
	UFUNCTION()
		virtual void RefreshServerList() override;

	// son blueprint callables porque se los llama desde el level blueprint. 
	UFUNCTION(BlueprintCallable)
		void LoadMenu();
	UFUNCTION(BlueprintCallable)
		void LoadPauseMenu();

	void StartSession();

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	class UUW_MainMenu* MainMenu;
	TSubclassOf<UUserWidget> PauseMenuClass;
	class UBaseMenuWidget* PauseMenu;

	FString TravelURL = "/Game/Maps/M_Lobby?listen";

	// online match
	IOnlineSessionPtr OnlineSessionInterface;

	TSharedPtr <class FOnlineSessionSearch> OnlineSessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Succeed);
	void OnDestroySessionComplete(FName SessionName, bool Succeed);
	void OnFindSessionsComplete(bool Succeed);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FString DesiredServerName;
	void CreateSession();
};
