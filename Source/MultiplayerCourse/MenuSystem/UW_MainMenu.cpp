// Fill out your copyright notice in the Description page of Project Settings.

#include "UW_MainMenu.h"
#include "Components/Button.h"
#include "Uobject/ConstructorHelpers.h"
#include "W_ServerListItem.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"


UUW_MainMenu::UUW_MainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget>BPServerListItemClass(TEXT("/Game/UI/UWG_Serveritem"));
	if (BPServerListItemClass.Class)
	{
		ServerListItemClass = BPServerListItemClass.Class;
	}
}

// inicializa el widget y crea el binding con la funcion hostServer y al menu de join server.
bool UUW_MainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	if (!ensure(BTN_Host != nullptr)) return false;
	BTN_Host->OnClicked.AddDynamic(this, &UUW_MainMenu::OpenHostMenu);
		if (!ensure(BTN_CreateSession != nullptr)) return false;
	BTN_CreateSession->OnClicked.AddDynamic(this, &UUW_MainMenu::HostServer);
	if (!ensure(BTN_Join != nullptr)) return false;
	BTN_Join->OnClicked.AddDynamic(this, &UUW_MainMenu::OpenJoinMenu);
	if (!ensure(BTN_Cancel != nullptr)) return false;
	BTN_Cancel->OnClicked.AddDynamic(this, &UUW_MainMenu::BackToMainMenu);
	BTN_HostCancel->OnClicked.AddDynamic(this, &UUW_MainMenu::BackToMainMenu);
	if(!ensure(BTN_Connect !=nullptr))return false;
	BTN_Connect->OnClicked.AddDynamic(this, &UUW_MainMenu::JoinServer);
	return true;
}

// llama a la funcion de la interface.
void UUW_MainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		FString newServerName = txt_NameServer->Text.ToString();
		MenuInterface->Host(newServerName);
	}
}
// abre el menu de join -> ip address call.
void UUW_MainMenu::OpenJoinMenu()
{
 	if (!ensure(JoinWidgetSwitcher != nullptr)) return;
 	if (!ensure(JoinMenu != nullptr))return;
	JoinWidgetSwitcher->SetActiveWidget(JoinMenu);
	if(MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UUW_MainMenu::OpenHostMenu()
{
	if (!ensure(JoinWidgetSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr))return;
	JoinWidgetSwitcher->SetActiveWidget(HostMenu);
}

void UUW_MainMenu::BackToMainMenu()
{
	if (!ensure(JoinWidgetSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr))return;
	JoinWidgetSwitcher->SetActiveWidget(MainMenu);
}

void UUW_MainMenu::JoinServer()
{
	if(SelectedIndex.IsSet())
	{
		if (MenuInterface != nullptr)
		{
			MenuInterface->Join(SelectedIndex.GetValue());
		}
	}
}
void UUW_MainMenu::SetServerListItems(TArray<FServerData>newNames)
{
	UWorld* World = this->GetWorld();
	ServerList->ClearChildren();
	uint32 Localindex=0;
	for(const FServerData& ServerData : newNames)
	{	
		UW_ServerListItem* Item = CreateWidget<UW_ServerListItem>(World, ServerListItemClass);
		Item->ServerItem->SetText(FText::FromString(ServerData.ServerName));
		Item->ServerItem->SetText(FText::FromString(ServerData.HostName));
		Item->NumPlayers->SetText(FText::AsNumber(ServerData.CurrentPlayers));
		Item->MaxPlayers->SetText(FText::AsNumber(ServerData.MaxPlayers));
		Item->Setup(this, Localindex);
		ServerList->AddChild(Item);
		++Localindex;
	}
}

void UUW_MainMenu::SetSelectedIndex(uint32 newIndex)
{
	SelectedIndex = newIndex;
	UpdateServerListChildren();
}

void UUW_MainMenu::UpdateServerListChildren()
{
	for(int32 i=0;i < ServerList->GetChildrenCount();++i)
	{
		auto CurrentItem = Cast<UW_ServerListItem>(ServerList->GetChildAt(i));
		if(CurrentItem != nullptr)
		{
			CurrentItem->bIsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
