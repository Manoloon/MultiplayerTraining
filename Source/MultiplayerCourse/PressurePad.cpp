// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePad.h"
#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APressurePad::APressurePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(100.0f,100.0f,200.0f));
	RootComponent = BoxComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APressurePad::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APressurePad::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &APressurePad::OnOverlapEnd);
}

// Called every frame
void APressurePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePad::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for(AMovingPlatform* newPlatform : PlatformsToTrigger)
	{
		newPlatform->AddActivePad();
	}
}

void APressurePad::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* newPlatform : PlatformsToTrigger)
	{
		newPlatform->RemoveActivePad();
	}
}

