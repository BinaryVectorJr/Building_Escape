// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT //this is being done, so as to remind us in GetPlayerViewPoint that the parameters that we pass into the function are technically outputs


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

///Look for attatched physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

///Look for attatched input components
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s input component found"), *GetOwner()->GetName())

		///Bind keypress action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s input component not found"), *GetOwner()->GetName())
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))

	///First LINE TRACE and then, we search for the objects with the proper object type i.e. PhysicsBody
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();
		auto ActorRotation = ActorHit->GetActorRotation();
		if (ActorHit && PhysicsHandle)
		{	///Later when we right click on an object, then intro a temp physics handle on it
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				ComponentToGrab,
				NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				ActorRotation
			);
		}
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"))

	///when we release right click, then destroy the temp. physics handle
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,	//we can indent the functions like this. just remember to keep the proper syntax
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//if the physics handle is attatched, then move the object each frame
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,	//we can indent the functions like this. just remember to keep the proper syntax
		OUT PlayerViewPointRotation
	);



	/*basically PVPLocation is a unit vector i.e. a dot say, and rotation is the direction component,
	and since it it multiplied by 100, it extends in whichever direction the body is looking*/
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	///Collision Query Parameters
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, GetOwner());

	///Get the PhysicsBody objects in the scene
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		CollisionParams
	);

	///See what we hit
	AActor* ObjectHit = Hit.GetActor();
	if (ObjectHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ObjectHit->GetName()));
	}
	return Hit;
}

///-------------------------------------------------GARBAGE AREA-----------------------------------------------///
/*Draw a red line to visualize the ray trace line
DrawDebugLine(
GetWorld(),
PlayerViewPointLocation,
LineTraceEnd,
FColor(255, 0, 0),
false,
.0f,
.0f,
10.0f
);

UE_LOG(LogTemp, Warning, TEXT("Position: %s and Rotation: %s"),
*PlayerViewPointLocation.ToString(),
*PlayerViewPointRotation.ToString()
); //to log the changes
*/
