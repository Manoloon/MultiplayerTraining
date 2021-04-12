// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class MULTIPLAYERCOURSE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(EditAnywhere)
		float Speed = 20;
	UPROPERTY(EditAnywhere,meta = (MakeEditWidget = true))
		FVector TargetLocation;

	void AddActivePad();
	void RemoveActivePad();

private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere)
		int ActivePads = 1;



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
