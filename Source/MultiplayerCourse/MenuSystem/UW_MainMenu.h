// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenuWidget.h"
#include "UW_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERCOURSE_API UUW_MainMenu : public UBaseMenuWidget
{
	GENERATED_BODY()
public:
	UUW_MainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerListItems(TArray<FString>newNames);

	void SetSelectedIndex(uint32 newIndex);

protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* BTN_Join;
	UPROPERTY(meta = (BindWidget))
		class UButton* BTN_Host;
 	UPROPERTY(meta = (BindWidget))
 		class UButton* BTN_Connect;
 	UPROPERTY(meta = (BindWidget))
 		class UButton* BTN_Cancel;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* JoinWidgetSwitcher;
 	UPROPERTY(meta = (BindWidget))
 		class UPanelWidget* ServerList;

	// estos son los widgets que estan dentro del switcher.
 	UPROPERTY(meta = (BindWidget))
 		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	TSubclassOf<UUserWidget> ServerListItemClass;

	UFUNCTION()
		void HostServer();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void BackToMainMenu();
	UFUNCTION()
		void JoinServer();

	// TOptional retorna un valor si no es nullptr , si lo es, no retorna nada y por ende no reserva memoria en el heap. 
	// se puede usar un uint32 basico con un valor en negativo PERO Toptional nos avisa si el valor es un nullptr o no y eso esta bueno.
	TOptional<uint32>SelectedIndex;
};
