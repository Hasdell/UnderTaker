// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MEPlayerState.h"
#//include "Multiplayer/MEGameInstance.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Weapon/Base/MEPlayerWeapon.h"

#include "Net/UnrealNetwork.h"

AMEPlayerState::AMEPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;

	IsInitialized = false;
}

void AMEPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEPlayerState, PlayerCharacter);

	DOREPLIFETIME(AMEPlayerState, KillCount);
	DOREPLIFETIME(AMEPlayerState, HeadShotCount);
	DOREPLIFETIME(AMEPlayerState, TotalDamage);
	DOREPLIFETIME(AMEPlayerState, SupportCount);
	DOREPLIFETIME(AMEPlayerState, KnockDownCount);

	DOREPLIFETIME(AMEPlayerState, PlayTime);
	DOREPLIFETIME(AMEPlayerState, PlayTimeString);
}

void AMEPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GetGameInstance<UMEAdvancedFriendsGameInstance>();
}

void AMEPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInitialized == false)
	{
		ME_INVALID_RETURN(PlayerCharacter);

		if (GetNetMode() == ENetMode::NM_Client && IsValid(PlayerCharacter->GetController()))
		{
			//ServerSetPlayerName(GameInstance->GetPlayerName());

			IsInitialized = true;
		}
	}
	PlayTime += DeltaTime;
}

void AMEPlayerState::ServerSetPlayerName_Implementation(FName name)
{
	MulticastSetPlayerName(name);
}

void AMEPlayerState::MulticastSetPlayerName_Implementation(FName name)
{
	SetPlayerName(name.ToString());
}

void AMEPlayerState::AddPlayerKillCount()
{
	KillCount++;
}
void AMEPlayerState::AddPlayerHeadShotCount()
{
	HeadShotCount++;
}
void AMEPlayerState::AddPlayerTotalDamage(float Damage)
{
	TotalDamage += Damage;
}
void AMEPlayerState::AddPlayerSupportCount()
{
	SupportCount++;
}
void AMEPlayerState::ServerAddPlayerKnockDownCount_Implementation()
{
	KnockDownCount++;
}
FString AMEPlayerState::GetPlayTime()
{
	int PlayHour = (int)PlayTime / 3600;
	int PlayMinute = (int)PlayTime % 3600 / 60;
	int PlaySecond = (int)PlayTime % 60;

	if (PlayHour == 0)
	{
		if (PlayMinute < 10)
		{
			PlayTimeString = FString::Printf(TEXT("00:0%d:%d"), PlayMinute, PlaySecond);
		}
		else
		{
			PlayTimeString = FString::Printf(TEXT("00:%d:%d"), PlayMinute, PlaySecond);
		}
	}
	else if (PlayMinute == 0)
	{
		if (PlaySecond < 10)
		{
			PlayTimeString = FString::Printf(TEXT("00:00:0%d"), PlaySecond);
		}
		else
		{
			PlayTimeString = FString::Printf(TEXT("00:00:%d"), PlaySecond);
		}
	}
	else
	{
		if (PlayHour < 10)
		{
			PlayTimeString = FString::Printf(TEXT("0%d:%d:%d"), PlayHour, PlayMinute, PlaySecond);
		}
		else
		{
			PlayTimeString = FString::Printf(TEXT("%d:%d:%d"), PlayHour, PlayMinute, PlaySecond);
		}
	}

	return PlayTimeString;
}