// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	CloseDoor();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	float OpenAngle = 200.0f;
	AActor* DoorOwner = GetOwner(); //To  get actor
	DoorOwner -> SetActorRotation(FRotator(0.f, OpenAngle, 0.f));

	FString NewDoorRotation = GetOwner()->GetActorRotation().ToString();
	///UE_LOG(LogTemp, Warning, TEXT("Rotator Hinge: %s"), *NewDoorRotation); //to log the changes
}

void UOpenDoor::CloseDoor()
{
	AActor* DoorOwner = GetOwner(); //To  get actor
	DoorOwner -> SetActorRotation(FRotator(0.f, 90.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(); //If ActorThatOpens is in the volume, then open the door
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

