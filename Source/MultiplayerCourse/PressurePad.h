// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMovingPlatform;

UCLASS()
class MULTIPLAYERCOURSE_API APressurePad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePad();

private:
	UPROPERTY(EditAnywhere, Category = "Component")
		UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Component")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<class AMovingPlatform*> PlatformsToTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
