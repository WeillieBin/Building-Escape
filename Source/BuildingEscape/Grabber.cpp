// Copyright Chris Weill 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();
}
//Bind Keys for Grab
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Input Component not found"), *GetOwner()->GetName());
	}
}
//Check for Physics Component
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Physics Handle not found"), *GetOwner()->GetName());
	}
}
//Pick up objects
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//Try and reach actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	///If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}
//Drop objects
void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
}
//Get LineTrace End Point
FVector UGrabber::LineTraceEndPoint()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	//Set Player Max Reach Position
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
//Get LineTrace Start Point
FVector UGrabber::LineTraceStartPoint()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	//Set Player View Point Position
	return PlayerViewPointLocation;
}
//Get Nearest Physics Body
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	LineTraceEndPoint();
	
	//Setup Query Params	
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Ray-cast to reach distance and find objects
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, LineTraceStartPoint(), LineTraceEndPoint(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object: %s"), *(ActorHit->GetName()));
	}
	return HitResult;
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if physics handle attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEndPoint());
	}
}

