// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/TriggerObject/METravelLevelTriggerObject.h"

#include "Multiplayer/MEGameModeBase.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Net/UnrealNetwork.h"

AMETravelLevelTriggerObject::AMETravelLevelTriggerObject()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMETravelLevelTriggerObject::OnTriggerBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMETravelLevelTriggerObject::OnTriggerEnd);
}
void AMETravelLevelTriggerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		if (PlayerList.Num() == GameMode->GetPlayerAliveCountNum() && GameMode->GetPlayerNum() != 0)
		{
			bIsCalledTravelLevel = true;
		}

		if (bIsCalledTravelLevel)
		{
			if (IsValid(PlayerList[0]))
			{
				PlayerList[0]->TravelLevelToBossLevel();
				SetActorTickEnabled(false);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("PlayerIsEmpty"));
			}
			bIsCalledTravelLevel = false;
		}
	}
}
void AMETravelLevelTriggerObject::OnTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		ServerPlayerListAdd(Player);
	}
}
void AMETravelLevelTriggerObject::OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMEPlayerCharacter>(OtherActor))
	{
		AMEPlayerCharacter* Player = Cast<AMEPlayerCharacter>(OtherActor);
		ServerPlayerListRemove(Player);
	}
}
void AMETravelLevelTriggerObject::ServerPlayerListAdd_Implementation(AMEPlayerCharacter* Player)
{
	MultiCastPlayerListAdd(Player);
}
void AMETravelLevelTriggerObject::ServerPlayerListRemove_Implementation(AMEPlayerCharacter* Player)
{
	MultiCastPlayerListRemove(Player);
}
void AMETravelLevelTriggerObject::MultiCastPlayerListAdd_Implementation(AMEPlayerCharacter* Player)
{
	PlayerList.Add(Player);
}
void AMETravelLevelTriggerObject::MultiCastPlayerListRemove_Implementation(AMEPlayerCharacter* Player)
{
	PlayerList.Remove(Player);
}
void AMETravelLevelTriggerObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMETravelLevelTriggerObject, PlayerList);
}