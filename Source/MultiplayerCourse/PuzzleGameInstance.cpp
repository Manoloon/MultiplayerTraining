// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGameInstance.h"
#include "Uobject/ConstructorHelpers.h"
#include "MenuSystem/BaseMenuWidget.h"
#include "MenuSystem/UW_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

const static FName SESSION_NAME = TEXT("Horde Play Session");

UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBaseMenuWidget>BPMainMenuClass(TEXT("/Game/UI/WG_MainMenu"));
	if(BPMainMenuClass.Class)
	{
		MainMenuClass = BPMainMenuClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UBaseMenuWidget>BPPauseMenuClass(TEXT("/Game/UI/WG_PauseMenu"));
	if (BPPauseMenuClass.Class)
	{
		PauseMenuClass = BPPauseMenuClass.Class;
	}
}

void UPuzzleGameInstance::Init()
{
	IOnlineSubsystem* MyOnlineSubSystem = IOnlineSubsystem::Get();
	if (MyOnlineSubSystem != nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem %s"), *MyOnlineSubSystem->GetSubsystemName().ToString());
		OnlineSessionInterface = MyOnlineSubSystem->GetSessionInterface();
		if(OnlineSessionInterface.IsValid())
		{
			OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
			OnlineSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);
			OnlineSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnFindSessionsComplete);
			OnlineSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online Subsystem NOT FOUND"));
	}
}

void UPuzzleGameInstance::Host()
{
	if(OnlineSessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = OnlineSessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession !=nullptr)
		{
			OnlineSessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzleGameInstance::Join(uint32 newIndex)
{
	if (!OnlineSessionInterface.IsValid()) { return; }
	if (!OnlineSessionSearch.IsValid()) { return; }
	if(MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}
	OnlineSessionInterface->JoinSession(0, SESSION_NAME, OnlineSessionSearch->SearchResults[newIndex]);

// 	UEngine* Engine = GetEngine();
// 	if (!ensure(Engine != nullptr)) return;
// 	Engine->AddOnScreenDebugMessage(0,2.0f,FColor::Green,FString::Printf(TEXT("Joining to : %s"), *RemoteAddress));
// 
// 	APlayerController* PlayerController = GetFirstLocalPlayerController();
// 	if (!ensure(PlayerController != nullptr))return;
// 
// 	PlayerController->ClientTravel(RemoteAddress,ETravelType::TRAVEL_Absolute);
}

void UPuzzleGameInstance::RefreshServerList()
{
	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch);
	OnlineSessionSearch->bIsLanQuery = true;

	if (OnlineSessionSearch.IsValid())
	{
		OnlineSessionInterface->FindSessions(0, OnlineSessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Start Session Found"));
	}
}

void UPuzzleGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/Maps/M_MainMenu", ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzleGameInstance::LoadMenu()
{
	if (MainMenuClass)
	{
		MainMenu = CreateWidget<UUW_MainMenu>(this, MainMenuClass);
		if (MainMenu == nullptr) { return; }
		MainMenu->Setup();
		MainMenu->SetMenuInterface(this);
	}
}

void UPuzzleGameInstance::LoadPauseMenu()
{
	if (PauseMenuClass)
	{
		PauseMenu = CreateWidget<UBaseMenuWidget>(this, PauseMenuClass);
		if (PauseMenu == nullptr) { return; }
		PauseMenu->Setup();
		PauseMenu->SetMenuInterface(this);
	}
}

// callbacks and online functions.

void UPuzzleGameInstance::OnFindSessionsComplete(bool Succeed)
{
	UE_LOG(LogTemp, Warning, TEXT("Finish Session Found"));
	if(Succeed && OnlineSessionSearch.IsValid() && MainMenu !=nullptr)
	{
		TArray<FOnlineSessionSearchResult> Results = OnlineSessionSearch->SearchResults;
		TArray<FString>ServerNames;
		for(FOnlineSessionSearchResult& Result : Results)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session Found %s"),*Result.GetSessionIdStr());
			ServerNames.Add(Result.GetSessionIdStr());
		}
		MainMenu->SetServerListItems(ServerNames);
	}
}

void UPuzzleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid()) { return; }
	FString RemoteSession;
	if(!OnlineSessionInterface->GetResolvedConnectString(SessionName, RemoteSession))
	{
		UE_LOG(LogTemp, Error, TEXT("No Valid Remote Session"));
		return;
	}
	else
	{
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr))return;

		PlayerController->ClientTravel(RemoteSession, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzleGameInstance::CreateSession()
{
		FOnlineSessionSettings OnlineSessionSettings;
		OnlineSessionSettings.bIsLANMatch = true;
		OnlineSessionSettings.NumPublicConnections = 4;
		OnlineSessionSettings.bShouldAdvertise = true;
		OnlineSessionInterface->CreateSession(0, SESSION_NAME, OnlineSessionSettings);
}


void UPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeed)
{
	if (!Succeed)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create session"));
		return;
	}
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, TEXT("Hosting"));
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/M_Playground?listen");
}

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeed)
{
	if (Succeed)
	{
		CreateSession();
	}
}