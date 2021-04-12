// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"

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
		virtual void Host() override;
	UFUNCTION()
		virtual void Join(const FString& RemoteAddress) override;
	UFUNCTION()
		virtual void LoadMainMenu() override;

	// son blueprint callables porque se los llama desde el level blueprint. 
	UFUNCTION(BlueprintCallable)
		void LoadMenu();
	UFUNCTION(BlueprintCallable)
		void LoadPauseMenu();

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	class UBaseMenuWidget* MainMenu;
	TSubclassOf<UUserWidget> PauseMenuClass;
	UBaseMenuWidget* PauseMenu;

	// online match
	IOnlineSessionPtr OnlineSessionInterface;

	TSharedPtr <class FOnlineSessionSearch> OnlineSessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Succeed);
	void OnDestroySessionComplete(FName SessionName, bool Succeed);
	void OnFindSessionsComplete(bool Succeed);
	void CreateSession();
};
