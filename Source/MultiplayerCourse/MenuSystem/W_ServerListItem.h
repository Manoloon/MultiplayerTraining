// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ServerListItem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERCOURSE_API UW_ServerListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerItem;
	
};
