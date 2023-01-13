// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Multiplayer/MEGameModeBase.h"
#include "MEBaseCampGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEBaseCampGameMode : public AMEGameModeBase
{
	GENERATED_BODY()
public:
	AMEBaseCampGameMode();

public:
	virtual void PreInitializeComponents() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(class APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Logout(AController* Exiting) override;
public:
	UFUNCTION(Server, Reliable)
		void ServerChangeCharacter(class AMEBaseCampController* Exiting);
		void ServerChangeCharacter_Implementation(class AMEBaseCampController* Exiting) { MultiCastChangeCharacter(Exiting); }
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastChangeCharacter(class AMEBaseCampController* Exiting);
public:
	void SpawnAnna(class AMEBaseCampController* Exiting);
	void SpawnAlan(class AMEBaseCampController* Exiting);
private:
	void SpawnPlayer(class APlayerController* NewPlayer) override;
	
};
