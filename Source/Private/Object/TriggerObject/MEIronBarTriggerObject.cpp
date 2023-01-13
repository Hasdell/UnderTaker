// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/TriggerObject/MEIronBarTriggerObject.h"

#include "Object/InteractionObject/MEIronBar.h"

#include "Multiplayer/MEGameModeBase.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Net/UnrealNetwork.h"

#include "Enemy/Monster/Basscard/MEBasscardCharacter.h"

AMEIronBarTriggerObject::AMEIronBarTriggerObject()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMEIronBarTriggerObject::OnTriggerBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMEIronBarTriggerObject::OnTriggerEnd);
}

void AMEIronBarTriggerObject::ServerPlayerListAdd_Implementation(class AMEPlayerCharacter* Player)
{
	MultiCastPlayerListAdd(Player);
}

void AMEIronBarTriggerObject::ServerPlayerListRemove_Implementation(class AMEPlayerCharacter* Player)
{
	MultiCastPlayerListRemove(Player);
}

void AMEIronBarTriggerObject::MultiCastPlayerListAdd_Implementation(class AMEPlayerCharacter* Player)
{
	PlayerList.Add(Player);
}

void AMEIronBarTriggerObject::MultiCastPlayerListRemove_Implementation(class AMEPlayerCharacter* Player)
{
	PlayerList.Remove(Player);
}


void AMEIronBarTriggerObject::OnTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		ServerPlayerListAdd(Player);
	}
}
void AMEIronBarTriggerObject::OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		ServerPlayerListRemove(Player);
	}
}

void AMEIronBarTriggerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		if (TriggerType == ETriggerType::AllPlayer)
		{
			if (PlayerList.Num() == GameMode->GetPlayerAliveCountNum() && GameMode->GetPlayerNum() != 0)
			{
				bIsCalledIronBarOperation = true;
			}
		}
		else if (TriggerType == ETriggerType::BasscardDead)
		{
			if (!IsValid(Basscard))
				return;

			if (Basscard->GetStatComponent()->GetCurrentHP() == 0)
			{
				bIsCalledIronBarOperation = true;
			}
		}

		if (bIsCalledIronBarOperation)
		{
			if (IsValid(IronBar))
			{
				IronBar->MultiCastIronBarOperaction();

				UE_LOG(LogTemp, Log, TEXT("Operaction"));
				SetActorTickEnabled(false);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("IronBarIsEmpty"));
			}
			bIsCalledIronBarOperation = false;
		}
	}
}
void AMEIronBarTriggerObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEIronBarTriggerObject, PlayerList);

	DOREPLIFETIME(AMEIronBarTriggerObject, bIsCalledIronBarOperation);
}

