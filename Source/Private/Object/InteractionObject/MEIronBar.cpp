// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEIronBar.h"

#include "Net/UnrealNetwork.h"

#include "FMODBlueprintStatics.h"

AMEIronBar::AMEIronBar()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetCollisionProfileName(TEXT("Item"));
	RootComponent = StaticMesh;

	FMODOperationSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("OperationSoundComponent"));
	FMODStopSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("StopSoundComponent"));
	
	FMODOperationSoundComponent->SetupAttachment(RootComponent);
	FMODStopSoundComponent->SetupAttachment(RootComponent);

	Accel = 1;
	
}
void AMEIronBar::ServerIronBarStop_Implementation()
{
	bIsIronBarOperaction = false;

	//Need MultiCast
	MultiCastPlayStopSound();
}
void AMEIronBar::MultiCastIronBarOperaction_Implementation()
{
	bIsIronBarOperaction = true;

	FMODOperationSoundComponent->Play();

	if (OperationMode == EIronBarOperationMode::Normal)
	{
		if (IronBarMoveDirection == EObjectMoveDirection::Up)
		{
			MaxMoveZVector = GetActorLocation().Z + MaxMoveDistance;
		}
		else
		{
			MaxMoveZVector = GetActorLocation().Z - MaxMoveDistance;
		}
	}
	if (OperationMode == EIronBarOperationMode::Defense)
	{
		if (Progress >= 100.f)
		{
			return;
		}
		else
		{
			Progress++;
		}

		bIsIronBarOperaction = true;
	}
}
bool AMEIronBar::GetIsOperaction()
{
	return bIsIronBarOperaction;
}
EIronBarOperationMode AMEIronBar::GetOperationMode()
{
	return OperationMode;
}
EObjectMoveDirection AMEIronBar::GetMoveDirection()
{
	return IronBarMoveDirection;
}
int	AMEIronBar::GetProgress()
{
	return Progress;
}
void AMEIronBar::ServerSetMoveDirection_Implementation(EObjectMoveDirection MoveDirection)
{
	MultiCastSetMoveDirection(MoveDirection);
}
void AMEIronBar::MultiCastSetMoveDirection_Implementation(EObjectMoveDirection MoveDirection)
{
	IronBarMoveDirection = MoveDirection;
}
// Called every frame
void AMEIronBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OperationMode == EIronBarOperationMode::Normal)
	{
		if (bIsIronBarOperaction)
		{
			if (IronBarMoveDirection == EObjectMoveDirection::Up)
			{
				NewLocation = GetActorLocation() + FVector(0, 0, Speed * (DeltaTime * Accel));
			}
			else
			{
				NewLocation = GetActorLocation() + FVector(0, 0, Speed * (DeltaTime * Accel) * (-1.f));
			}
			SetActorLocation(NewLocation);

			Accel *= 1.15;

			if (IronBarMoveDirection == EObjectMoveDirection::Up)
			{
				if (GetActorLocation().Z > MaxMoveZVector)
				{
					bIsIronBarOperaction = false;

					IronBarMoveDirection = EObjectMoveDirection::Down;

					Accel = 1;
				}
				
				FMODOperationSoundComponent->Stop();
				FMODStopSoundComponent->Play();
			}
			else
			{
				if (GetActorLocation().Z < MaxMoveZVector)
				{
					bIsIronBarOperaction = false;

					IronBarMoveDirection = EObjectMoveDirection::Up;

					Accel = 1;
				}

				FMODOperationSoundComponent->Stop();
				FMODStopSoundComponent->Play();
			}
		}
	}
	else if (OperationMode == EIronBarOperationMode::Defense)
	{
		if (bIsIronBarOperaction)
		{
			NewLocation = GetActorLocation() + FVector(0, 0, Speed * DeltaTime);

			SetActorLocation(NewLocation);

			if (!(Progress >= 100))
			{
				ProgressTimer += DeltaTime;
				if (ProgressTimer >= 1.f)
				{
					Progress++;
					ProgressTimer = 0.f;
				}
			}
		}
	}
}
void AMEIronBar::MultiCastSetActorLocation_Implementation()
{
	SetActorLocation(NewLocation);
}
void AMEIronBar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEIronBar, IronBarMoveDirection);
	DOREPLIFETIME(AMEIronBar, bIsIronBarOperaction);
	DOREPLIFETIME(AMEIronBar, NewLocation);

	DOREPLIFETIME(AMEIronBar, Accel);
	DOREPLIFETIME(AMEIronBar, MaxMoveZVector);

	DOREPLIFETIME(AMEIronBar, Progress);
}

void AMEIronBar::MultiCastPlayStopSound_Implementation()
{
	FMODOperationSoundComponent->Stop();
	FMODStopSoundComponent->Play();
}