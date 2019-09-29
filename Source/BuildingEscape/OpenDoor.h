// Copyright Chris Weill 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h" 
#include "OpenDoor.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();
	void CloseDoor();
	float TotalMass();

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = -175.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	AActor *Owner;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.7f;

	float LastDoorOpenTime;

	float TriggerMassThreshold = 19.f;		
};
