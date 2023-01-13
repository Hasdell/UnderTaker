// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEElevatorLever.h"

#include "Object/InteractionObject/MEElevatorWall.h"
#include "Object/InteractionObject/MEElevatorFloor.h"
#include "Multiplayer/MEGameModeBase.h"
#include "Net/UnrealNetwork.h"

AMEElevatorLever::AMEElevatorLever()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetCollisionProfileName(TEXT("Item"));
	RootComponent = StaticMesh;

}
void AMEElevatorLever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMEElevatorLever, ElevatorWall);
	DOREPLIFETIME(AMEElevatorLever, ElevatorFloor);
}
void AMEElevatorLever::ServerInteraction_Implementation()
{
	if (ElevatorFloor->IsAllPlayerGetOnElevator())
	{
		ElevatorFloor->MultiCastElevatorOperation();
		ElevatorWall->MultiCastElevatorOperation();

		bCanInteraction = false;
	}
}
void AMEElevatorLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanInteraction)
	{
		InteractionTimer += DeltaTime;

		if (InteractionTimer >= InteractionCoolTime)
		{
			bCanInteraction = true;

			InteractionTimer = 0.f;
		}
	}
}