// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEIronDoor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Net/UnrealNetwork.h"

#include "Components/WidgetComponent.h"

#include "FMODBlueprintStatics.h"

// Sets default values
AMEIronDoor::AMEIronDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Sphere;

	BridgeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BridgeCollision->SetupAttachment(Sphere);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(Sphere);

	FMODOpenDoorSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("OpenDoorSoundComponent"));
	FMODOpenDoorSoundComponent->SetupAttachment(RootComponent);

	InteractionWidgetComponent->SetupAttachment(StaticMesh);
	MarkWidgetComponent->SetupAttachment(StaticMesh);
}	

void AMEIronDoor::ServerInteraction_Implementation()
{
	if (bCanInteraction)
	{
		bIsInteraction = true;

		bCanInteraction = false;

		if (IronDoorMoveDirection == EIronDoorMoveDirection::Forward)
		{
			MaxRotatorYaw = GetActorRotation().Yaw + RotationAngle;
		}
		else
		{
			MaxRotatorYaw = GetActorRotation().Yaw - RotationAngle;
		}

		Accel = 1;

		//need MultiCast
		MultiCastPlayOpenDoorSound();
	}
}
// Called every frame
void AMEIronDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInteraction)
	{
		if (IronDoorMoveDirection == EIronDoorMoveDirection::Forward)
		{
			NewRotation = GetActorRotation() + FRotator(0.f, RotationSpeed * (DeltaTime * Accel), 0.f);
		}
		else
		{
			NewRotation = GetActorRotation() + FRotator(0.f, RotationSpeed * (DeltaTime * Accel) * -1.f, 0.f);
		}
		
		SetActorRotation(NewRotation);

		Accel *= 1.15;

		if (IronDoorMoveDirection == EIronDoorMoveDirection::Forward)
		{
			if (GetActorRotation().Yaw > MaxRotatorYaw)
			{
				bIsInteraction = false;
			}
		}
		else
		{
			if (GetActorRotation().Yaw < MaxRotatorYaw)
			{
				bIsInteraction = false;
			}
		}
	}
}
void AMEIronDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEIronDoor, Accel);
	DOREPLIFETIME(AMEIronDoor, bIsInteraction);
	DOREPLIFETIME(AMEIronDoor, NewRotation);
	DOREPLIFETIME(AMEIronDoor, MaxRotatorYaw);
}

void AMEIronDoor::MultiCastPlayOpenDoorSound_Implementation()
{
	FMODOpenDoorSoundComponent->Play();
}
