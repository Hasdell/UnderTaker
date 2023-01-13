// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameBase/MoonEater.h"
#include "GameFramework/PlayerState.h"
#include "Multiplayer/MEAdvancedFriendsGameInstance.h"
#include "MEPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public :
	AMEPlayerState();

public :
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

public :
	UFUNCTION(Server, Reliable)
	void ServerSetPlayerName(FName name);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetPlayerName(FName name);

	UFUNCTION(Server, Reliable)
		void ServerAddPlayerKnockDownCount();

	void AddPlayerKillCount();
	void AddPlayerHeadShotCount();
	void AddPlayerTotalDamage(float Damage);
	void AddPlayerSupportCount();

	UFUNCTION(BlueprintCallable)
	FString GetPlayTime();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE	int GetPlayerKillCount() { return KillCount; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE	int GetPlayerHeadShotCount() { return HeadShotCount; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE	int GetPlayerTotalDamage() { return TotalDamage; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE	int GetPlayerSupportCount() { return SupportCount; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE	int GetPlayerKnockDownCount() { return KnockDownCount; }

	FORCEINLINE void SetPlayerCharacter(class AMEPlayerCharacter* PlayerChracterArg) { PlayerCharacter = PlayerChracterArg; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AMEPlayerCharacter* GetPlayerCharacter() { return PlayerCharacter; }

private :
	class UMEAdvancedFriendsGameInstance* GameInstance;

	UPROPERTY(Replicated)
	class AMEPlayerCharacter* PlayerCharacter;

	bool IsInitialized;

	UPROPERTY(Replicated)
	int KillCount;
	UPROPERTY(Replicated)
	int HeadShotCount;
	UPROPERTY(Replicated)
	int TotalDamage;
	UPROPERTY(Replicated)
	int SupportCount;
	UPROPERTY(Replicated)
	int KnockDownCount;

	UPROPERTY(Replicated)
	float PlayTime;
	UPROPERTY(Replicated)
	FString PlayTimeString;
};
