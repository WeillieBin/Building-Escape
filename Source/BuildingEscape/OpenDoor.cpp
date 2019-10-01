// Copyright Chris Weill 2019


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}
void UOpenDoor::OpenDoor()
{
	if (!Owner) {return; }
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}
void UOpenDoor::CloseDoor()
{
	if (!Owner) {return; }
	Owner->SetActorRotation(FRotator(0.f, -90.f, 0.f));
}
float UOpenDoor::TotalMass()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto* Actor : OverlappingActors)
{
    TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
}

	return TotalMass;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger Volume
	if (TotalMass() > TriggerMassThreshold)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
	
}


