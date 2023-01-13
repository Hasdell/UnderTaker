// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEElevatorFloor.h"

#include "Components/BoxComponent.h"

#include "Net/UnrealNetwork.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Multiplayer/MEGameModeBase.h"

AMEElevatorFloor::AMEElevatorFloor()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMesh;

	EntCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EntCollision"));
	EntCollision->SetupAttachment(RootComponent);
	ExitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitCollision"));
	ExitCollision->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMEElevatorFloor::OnTriggerBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMEElevatorFloor::OnTriggerEnd);
}

bool AMEElevatorFloor::IsAllPlayerGetOnElevator()
{
	if (PlayerList.Num() == GameMode->GetPlayerNum())
	{
		return true;
	}
	return false;
}

void AMEElevatorFloor::MultiCastElevatorOperation_Implementation()
{
	if (!bEnableDecel)
	{
		bElevatorOperation = true;
		EntCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ExitCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AMEElevatorFloor::MultiCastSetActorLocation_Implementation()
{
	SetActorLocation(NewLocation);
}
void AMEElevatorFloor::MultiCastEntExitCollisionDisable_Implementation()
{
	EntCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ExitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AMEElevatorFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		if (bElevatorOperation && CurrentTime > ElevatorSectionA)
		{
			if (CurrentSpeed < FloorMaxSpeed)
			{
				CurrentSpeed += (DeltaTime * 20);
			}
		}
		if (bEnableDecel)
		{
			if (CurrentSpeed > 0)
			{
				CurrentSpeed -= (DeltaTime * 20);
			}

		}

		if (MoveDirection == EObjectMoveDirection::Up)
		{
			NewLocation = GetActorLocation() + FVector(0.f, 0.f, CurrentSpeed * DeltaTime);

		}
		else
		{
			NewLocation = GetActorLocation() + FVector(0.f, 0.f, CurrentSpeed * DeltaTime * (-1.f));
		}

		if (bElevatorOperation)
		{
			ServerSetActorLocation();
			CurrentTime += DeltaTime;
		}

		if (CurrentTime >= ElevatorSectionB)
		{
			bElevatorOperation = false;
			bEnableDecel = true;

			CurrentTime = 0.f;
		}
		if (bEnableDecel && CurrentSpeed <= 0)
		{
			if (MoveDirection == EObjectMoveDirection::Up)
			{
				MoveDirection = EObjectMoveDirection::Down;
			}
			else
			{
				MoveDirection = EObjectMoveDirection::Up;
			}

			bEnableDecel = false;
			
			ServerEntExitCollisionDisable();

			CurrentTime = 0.f;
		}
	}
}
void AMEElevatorFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEElevatorFloor, CurrentSpeed);
	DOREPLIFETIME(AMEElevatorFloor, CurrentTime);

	DOREPLIFETIME(AMEElevatorFloor, bElevatorOperation);
	DOREPLIFETIME(AMEElevatorFloor, bEnableDecel);

	DOREPLIFETIME(AMEElevatorFloor, NewLocation);
}
void AMEElevatorFloor::OnTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		PlayerList.Add(Player);
	}
}
void AMEElevatorFloor::OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		PlayerList.Remove(Player);
	}
}
