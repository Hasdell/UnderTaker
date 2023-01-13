// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MEBaseCampController.h"
#include "UI/MEPlayerUIManager.h"
#include "Multiplayer/MEBaseCampGameMode.h"
#include "Multiplayer/MEAdvancedFriendsGameInstance.h"
#include "Net/UnrealNetwork.h"

AMEBaseCampController::AMEBaseCampController()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AMEBaseCampController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
void AMEBaseCampController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEBaseCampController, PlayerCharacter);
	DOREPLIFETIME(AMEBaseCampController, PrevCharacter);
	DOREPLIFETIME(AMEBaseCampController, PlayerUIManager);
}

void AMEBaseCampController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AMEBaseCampController::BeginPlay()
{
	Super::BeginPlay();

}
void AMEBaseCampController::ChangeCharacter()
{
	ClientUpdatePlayerSelectedCharacter();
	ClientSetBGMComponent();
}
void AMEBaseCampController::ClientUpdatePlayerSelectedCharacter_Implementation()
{
	auto GameInstance = GetGameInstance<UMEAdvancedFriendsGameInstance>();

	PlayerCharacterBP = GameInstance->GetPlayerCharacterBP();
	PlayerBaseWeaponBP = GameInstance->GetPlayerBaseWeaponBP();
	PlayerCrosshairWidgetBP = GameInstance->GetPlayerCrosshairWidgetBP();
	PlayerInGameWidgetBP = GameInstance->GetPlayerInGameWidgetBP();

	ServerUpdatePlayerSelectedCharacter(PlayerCharacterBP, PlayerBaseWeaponBP, PlayerCrosshairWidgetBP, PlayerInGameWidgetBP);
}

void AMEBaseCampController::ServerUpdatePlayerSelectedCharacter_Implementation(TSubclassOf<class AMEPlayerCharacter> NewCharacterBP, TSubclassOf<class AMEPlayerWeapon> NewBaseWeaponBP, TSubclassOf<class UUserWidget> NewCrosshairWidgetBP, TSubclassOf<class UUserWidget> NewInGameWidgetBP)
{
	PlayerCharacterBP = NewCharacterBP;
	PlayerBaseWeaponBP = NewBaseWeaponBP;
	PlayerCrosshairWidgetBP = NewCrosshairWidgetBP;
	PlayerInGameWidgetBP = NewInGameWidgetBP;

	GetWorld()->GetAuthGameMode<AMEBaseCampGameMode>()->ServerChangeCharacter(this);
}

void AMEBaseCampController::SetPrevCharacter(class AMEPlayerCharacter* PrC)
{
	PrevCharacter = PrC;
}

void AMEBaseCampController::PrevCharacterInit(int num)
{
	PrevCharacter->SetActorLocation(FVector(0.f, (num * 150.f), 0.f));
	PrevCharacter->SetActorTickEnabled(false);
}

void AMEBaseCampController::BaseCampSpawnAnna()
{
	ServerBaseCampSpawnAnna();
}

void AMEBaseCampController::ServerBaseCampSpawnAnna_Implementation()
{
	GetWorld()->GetAuthGameMode<AMEBaseCampGameMode>()->SpawnAnna(this);
}

void AMEBaseCampController::BaseCampSpawnAlan()
{
	ServerBaseCampSpawnAlan();
}

void AMEBaseCampController::ServerBaseCampSpawnAlan_Implementation()
{
	GetWorld()->GetAuthGameMode<AMEBaseCampGameMode>()->SpawnAlan(this);
}

void AMEBaseCampController::ClientSetBGMComponent_Implementation()
{
	UFMODAudioComponent* BGMComponent = PlayerCharacter->GetCurrentBGMComponent();
	if (!IsValid(PrevCharacter->GetCurrentBGMComponent()))
	{
		if (IsValid(BGMComponent))
		{
			PrevCharacter->Client_StartBGMEvent(BGMComponent->Event);
			PrevCharacter->GetCurrentBGMComponent()->SetTimelinePosition(BGMComponent->GetTimelinePosition());

			PlayerCharacter->Client_StopBGMEvent();
		}
	}
}