// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MEBaseCampGameMode.h"
#include "Multiplayer/MEBaseCampController.h"
#include "Multiplayer/MEPlayerState.h"
#include "Multiplayer/MEGameStateBase.h"
#include "Player/Base/MEPlayerSpectatorPawn.h"

#include "Weapon/Base/MEPlayerWeapon.h"

#include "UI/MEPlayerUIManager.h"

AMEBaseCampGameMode::AMEBaseCampGameMode()
{
	DefaultPawnClass = nullptr;
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AMEBaseCampController::StaticClass();
	PlayerStateClass = AMEPlayerState::StaticClass();
	GameStateClass = AMEGameStateBase::StaticClass();

}
void AMEBaseCampGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMEBaseCampGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	
}
void AMEBaseCampGameMode::PostLogin(class APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ServerSpawnAnna(Cast<AMEBaseCampController>(NewPlayer));
}
void AMEBaseCampGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AMEBaseCampGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
void AMEBaseCampGameMode::MultiCastChangeCharacter_Implementation(class AMEBaseCampController* Exiting)
{
	for (int i = 0; i < Controllers.Num(); i++)
	{
		if (Controllers[i] == Exiting)
		{
			FTransform ActorTransform = Players[i]->GetTransform();
			FRotator ActorRotation = Players[i]->GetActorRotation();
			
			FActorSpawnParameters ActorSpawnParam;

			ActorSpawnParam.bNoFail = true;
			ActorSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParam.Owner = Exiting;

			AMEPlayerCharacter* NewPlayerCharacter = nullptr;

			/*
			if (!Exiting->GetPrevCharacter())
			{
				NewPlayerCharacter = GetWorld()->SpawnActor<AMEPlayerCharacter>(Exiting->PlayerCharacterBP, ActorTransform, ActorSpawnParam);
				NewPlayerWeapon = GetWorld()->SpawnActor<AMEPlayerWeapon>(Exiting->PlayerBaseWeaponBP, ActorSpawnParam);

				NewPlayerCharacter->SetWeapon(NewPlayerWeapon);
				NewPlayerCharacter->SetUIManager(Exiting->PlayerUIManager);

				NewPlayerWeapon->SetPlayerCharacter(NewPlayerCharacter);

				Exiting->Possess(NewPlayerCharacter);

				Exiting->SetPrevCharacter(Players[i]);
				Exiting->PrevCharacterInit(i);
			}
			*/
			
			NewPlayerCharacter = Exiting->GetPrevCharacter();

			NewPlayerCharacter->SetActorTransform(ActorTransform);
			NewPlayerCharacter->SetActorTickEnabled(true);

			Exiting->Possess(NewPlayerCharacter);

			Exiting->SetPrevCharacter(Players[i]);
			Exiting->PrevCharacterInit(i);

			if (!IsValid(NewPlayerCharacter))
				return;

			Exiting->PlayerUIManager->SetPlayerCharacter(NewPlayerCharacter);
			Exiting->PlayerUIManager->SetCrosshairWidgetBlueprint(Exiting->PlayerCrosshairWidgetBP);
			Exiting->PlayerUIManager->SetInGameWidgetBlueprint(Exiting->PlayerInGameWidgetBP);

			Players[i] = NewPlayerCharacter;

			Exiting->PlayerCharacter = NewPlayerCharacter;

			break;
		}
	}
}

void AMEBaseCampGameMode::SpawnAnna(class AMEBaseCampController* Exiting)
{
	FTransform ActorTransform;
	FActorSpawnParameters ActorSpawnParam;

	ActorTransform.SetLocation(FVector(0.f, 0.f, 0.f));

	ActorSpawnParam.bNoFail = true;
	ActorSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParam.Owner = Exiting;

	auto GameInstance = GetGameInstance<UMEAdvancedFriendsGameInstance>();

	AMEPlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<AMEPlayerCharacter>(GameInstance->GetPlayerCharacterBP(1), ActorTransform, ActorSpawnParam);
	AMEPlayerWeapon* NewPlayerWeapon = GetWorld()->SpawnActor<AMEPlayerWeapon>(GameInstance->GetPlayerBaseWeaponBP(1), ActorSpawnParam);

	NewPlayerCharacter->SetWeapon(NewPlayerWeapon);
	NewPlayerCharacter->SetUIManager(Exiting->PlayerUIManager);

	NewPlayerWeapon->SetPlayerCharacter(NewPlayerCharacter);

	NewPlayerCharacter->SetActorTickEnabled(false);

	Exiting->SetPrevCharacter(NewPlayerCharacter);
}
void AMEBaseCampGameMode::SpawnAlan(class AMEBaseCampController* Exiting)
{
	FTransform ActorTransform;
	FActorSpawnParameters ActorSpawnParam;

	ActorTransform.SetLocation(FVector(0.f, 0.f, 0.f));

	ActorSpawnParam.bNoFail = true;
	ActorSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParam.Owner = Exiting;

	auto GameInstance = GetGameInstance<UMEAdvancedFriendsGameInstance>();

	AMEPlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<AMEPlayerCharacter>(GameInstance->GetPlayerCharacterBP(0), ActorTransform, ActorSpawnParam);
	AMEPlayerWeapon* NewPlayerWeapon = GetWorld()->SpawnActor<AMEPlayerWeapon>(GameInstance->GetPlayerBaseWeaponBP(0), ActorSpawnParam);

	NewPlayerCharacter->SetWeapon(NewPlayerWeapon);
	NewPlayerCharacter->SetUIManager(Exiting->PlayerUIManager);

	NewPlayerWeapon->SetPlayerCharacter(NewPlayerCharacter);

	NewPlayerCharacter->SetActorTickEnabled(false);

	Exiting->SetPrevCharacter(NewPlayerCharacter);
}
void AMEBaseCampGameMode::SpawnPlayer(class APlayerController* NewPlayer)
{
	Super::SpawnPlayer(NewPlayer);

	AMEBaseCampController* Exiting = Cast<AMEBaseCampController>(NewPlayer);

	auto GameInstance = GetGameInstance<UMEAdvancedFriendsGameInstance>();

	if (IsValid(Exiting->PlayerCharacterBP))
	{
		if (Exiting->PlayerCharacterBP == GameInstance->GetPlayerCharacterBP(0))
		{
			Exiting->BaseCampSpawnAnna();
		}
		else
		{
			Exiting->BaseCampSpawnAlan();
		}
	}
}