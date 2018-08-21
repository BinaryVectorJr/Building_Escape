// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"

#define OUT

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
	if (GetTotMassOfActorsOnPlate() > 20.f)		//TODO change it into a parameter later
	{
		OpenDoor(); //If ActorThatOpens is in the volume, then open the door
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if its time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//Add the weight of all overlapping actors
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" % on pressure plate!"), *Actor->GetName())
	}

	return TotalMass;
}

