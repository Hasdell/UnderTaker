// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEElevatorWall.h"
#include "Object/InteractionObject/MEElevatorTriggerComponent.h"

#include "Net/UnrealNetwork.h"

AMEElevatorWall::AMEElevatorWall()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	bElevatorOperation = false;
	CurrentElevatorSpeed = 0.f;

	NewLocation = FVector::ZeroVector;

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);
}
void AMEElevatorWall::MultiCastElevatorOperation_Implementation()
{
	if (!bEnableAccel && !bEnableDecel)
	{
		bElevatorOperation = true;
	}
}
void AMEElevatorWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		if (bElevatorOperation && CurrentTime > ElevatorSectionA)
		{
			if (CurrentElevatorSpeed < ElevatorSpeedB)
			{
				CurrentElevatorSpeed += (DeltaTime * 20);
			}
		}
		if (bEnableAccel)
		{
			if (CurrentElevatorSpeed < ElevatorSpeedB)
			{
				CurrentElevatorSpeed += (DeltaTime * 40);
			}
		}
		if (bEnableDecel)
		{
			if (CurrentElevatorSpeed > ElevatorSpeedC)
			{
				CurrentElevatorSpeed -= (DeltaTime * 20);
			}

		}

		if (MoveDirection == EObjectMoveDirection::Up)
		{
			NewLocation = GetActorLocation() + FVector(0.f, 0.f, CurrentElevatorSpeed * DeltaTime * (-1.f));

		}
		else
		{
			NewLocation = GetActorLocation() + FVector(0.f, 0.f, CurrentElevatorSpeed * DeltaTime);
		}

		if (bElevatorOperation || bEnableAccel || bEnableDecel)
		{
			ServerSetActorLocation();
			CurrentTime += DeltaTime;
		}

		if (CurrentTime >= ElevatorSectionB && bElevatorOperation)
		{
			bElevatorOperation = false;
			bEnableAccel = true;
			CurrentTime = 0.0f;
		}
		else if (CurrentTime >= ElevatorSectionC && bEnableAccel)
		{
			bEnableAccel = false;
			bEnableDecel = true;
			CurrentTime = 0.0f;
		}
		else if (bEnableDecel)
		{
			if (CurrentElevatorSpeed <= 0)
			{
				bEnableDecel = false;

				if (MoveDirection == EObjectMoveDirection::Up)
				{
					MoveDirection = EObjectMoveDirection::Down;
				}
				else
				{
					MoveDirection = EObjectMoveDirection::Up;
				}
				CurrentTime = 0.0f;
			}
		}
	}
}
void AMEElevatorWall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEElevatorWall, CurrentElevatorSpeed);
	DOREPLIFETIME(AMEElevatorWall, CurrentTime);

	DOREPLIFETIME(AMEElevatorWall, bElevatorOperation);
	DOREPLIFETIME(AMEElevatorWall, bEnableAccel);
	DOREPLIFETIME(AMEElevatorWall, bEnableDecel);

	DOREPLIFETIME(AMEElevatorWall, NewLocation);
}
void AMEElevatorWall::MultiCastSetActorLocation_Implementation()
{
	SetActorLocation(NewLocation);
}