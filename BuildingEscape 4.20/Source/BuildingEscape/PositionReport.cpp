// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include "Gameframework/Actor.h"


// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay(); //do everything till here i.e. alll the levels it has inherited upto here, our code starts from next line

	FString ObjectName = GetOwner()->GetName();
	FVector ObjectPos = GetOwner()->GetActorLocation(); //we can use ToString() here as well, just return type will have to be FString

	//Grey - Log, Yellow - Warning, Red - Error
	//remember the asterisk on ObjectName
	//.ToString() is used to convert a vector (or anything else) to string
	UE_LOG(LogTemp, Warning, TEXT("%s found on %s"), *ObjectName, *ObjectPos.ToString()); 
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

