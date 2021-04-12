// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	
}

void AMovingPlatform::AddActivePad()
{
	ActivePads++;
}

void AMovingPlatform::RemoveActivePad()
{
	if(ActivePads >0)
	{
		ActivePads--;
	}
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->SetMobility(EComponentMobility::Movable);

	SetReplicates(true);
	SetReplicateMovement(true);

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	if(ActivePads >0)
	{
		if (HasAuthority())
		{
			Super::Tick(DeltaTime);
			FVector Location = GetActorLocation();
			float MoveLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			float TravelledLength = (Location - GlobalStartLocation).Size();
			if (TravelledLength > MoveLength)
			{
				FVector SwapDestination = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = SwapDestination;
			}
			FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);
		}
	}	
}

