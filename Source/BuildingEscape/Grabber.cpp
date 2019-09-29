// Copyright Chris Weill 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	/* Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	off to improve performance if you don't need them.*/

	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
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
	if (PhysicsHandle)
	{
		//Physics Handle Is Attached
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Physics Handle not found"), *GetOwner()->GetName());
	}
}
//Pick up objects
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//Try and reach actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();
	//TODO If we hit something then attach a physics handle
}
//Drop objects
void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	//TODO Release Physics handle
}
//Get Nearest Physics Body
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
		
	//Set Player Max Reach Position
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	//Setup Query Params	
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Ray-cast to reach distance and find objects
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object: %s"), *(ActorHit->GetName()));
	}
	return FHitResult();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics handle attached
		//Move the object we are holding
}

