// Fill out your copyright notice in the Description page of Project Settings.

#include "UW_MainMenu.h"
#include "Components/Button.h"
#include "Uobject/ConstructorHelpers.h"
#include "W_ServerListItem.h"
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
	BTN_Host->OnClicked.AddDynamic(this, &UUW_MainMenu::HostServer);
	if (!ensure(BTN_Join != nullptr)) return false;
	BTN_Join->OnClicked.AddDynamic(this, &UUW_MainMenu::OpenJoinMenu);
	if (!ensure(BTN_Cancel != nullptr)) return false;
	BTN_Cancel->OnClicked.AddDynamic(this, &UUW_MainMenu::BackToMainMenu);
	if(!ensure(BTN_Connect !=nullptr))return false;
	BTN_Connect->OnClicked.AddDynamic(this, &UUW_MainMenu::JoinServer);
	return true;
}

// llama a la funcion de la interface.
void UUW_MainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}

}
// abre el menu de join -> ip address call.
void UUW_MainMenu::OpenJoinMenu()
{
 	if (!ensure(JoinWidgetSwitcher != nullptr)) return;
 	if (!ensure(JoinMenu != nullptr))return;
	JoinWidgetSwitcher->SetActiveWidget(JoinMenu);
}

void UUW_MainMenu::BackToMainMenu()
{
	if (!ensure(JoinWidgetSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr))return;
	JoinWidgetSwitcher->SetActiveWidget(MainMenu);
}

void UUW_MainMenu::JoinServer()
{
	if(MenuInterface !=nullptr)
	{
		UWorld* World = this->GetWorld();
		UW_ServerListItem* Item = CreateWidget<UW_ServerListItem>(World, ServerListItemClass);
		ServerList->AddChild(Item);		
	}
}
