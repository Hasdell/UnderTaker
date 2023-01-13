// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MEYugusUI.h"
#include "Components/TextBlock.h"
#include "GameBase/MoonEater.h"
#include "GameFramework/Actor.h"
#include "Multiplayer/MEGameStateBase.h"
#include "MEPlayerUIManager.generated.h"

UCLASS()
class MOONEATER_API AMEPlayerUIManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMEPlayerUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetGameState(AMEGameStateBase* State) { GameState = State; }
	
public:
	UFUNCTION(Client, Reliable)
	void ClientShowCrosshair();

	UFUNCTION(Client, Reliable)
	void ClientHideCrosshair();

	UFUNCTION(Client, Reliable)
	void ClientShowInGame();

	UFUNCTION(Client, Reliable)
	void ClientHideInGame();

	UFUNCTION(Client, Reliable)
	void ClientShowCrosshairShoot();

	UFUNCTION(Client, Reliable)
	void ClientHideCrosshairShoot();

	UFUNCTION(Client, Reliable)
	void ClientShowCrosshairHit();

	UFUNCTION(Client, Reliable)
	void ClientHideCrosshairHit();

	UFUNCTION(Client, Reliable)
	void ClientShowCrosshairHeadShot();
	UFUNCTION(Client, Reliable)
	void ClientHideCrosshairHeadShot();

	UFUNCTION(Client, Reliable)
	void ClientShowCrosshairReload();

	UFUNCTION(Client, Reliable)
	void ClientHideCrosshairReload();

	UFUNCTION(Client, Reliable)
	void ClientShowGetAttack();

	UFUNCTION(Client, Reliable)
	void ClientHideGetAttack();
	

	FORCEINLINE float GetCrosshairRadiusWorld() const {	return CrosshairRadiusWorld; }
	FORCEINLINE float GetCrosshairRadiusScreen() const { return CrosshairRadiusScreen; }

	FORCEINLINE void SetInGameUIOnOff(bool OnOff) { InGameUIOnOff = OnOff; }



	UFUNCTION(Server, UnReliable)
	void ServerSetCrosshairRadiusWorld(float CrosshairRadiusWorldArg);

	UFUNCTION(Server, UnReliable)
	void ServerSetCrosshairRadiusScreen(float CrosshairRadiusWorldArg);

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastSetCrosshairRadiusWorld(float CrosshairRadiusScreenArg);

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastSetCrosshairRadiusScreen(float CrosshairRadiusScreenArg);

	UFUNCTION(Client, Reliable)
	void ClientShowGameOver();

	UFUNCTION(Client, Reliable)
	void ClientShowGameClear();

	UFUNCTION(Client, Reliable)
	void ClientShowKnockedOut();

	UFUNCTION(Client, Reliable)
	void ClientHideKnockedOut();

	UFUNCTION(Client, Reliable)
	void ClientShowRevive();

	UFUNCTION(Client, Reliable)
	void ClientHideRevive();

	UFUNCTION(Client, Reliable)
		void ClientShowElevatorUI();

	UFUNCTION(Client, Reliable)
		void ClientHideElevatorUI();

	UFUNCTION(Client, Reliable)
	void ClientShowItemGet();

	UFUNCTION(Client, Reliable)
	void ClientHideItemGet();

	UFUNCTION(Client, Reliable)
	void ClientShowSpectator();

	UFUNCTION(Client, Reliable)
	void ClientHideSpectator();

	UFUNCTION(Client, Reliable)
	void ClientAddWidget(TSubclassOf<UUserWidget> WidgetBlueprint);

	FORCEINLINE void SetPlayerCharacter(class AMEPlayerCharacter* PlayerCharacterArg) { PlayerCharacter = PlayerCharacterArg; }
	FORCEINLINE void SetCrosshairWidgetBlueprint(TSubclassOf<UUserWidget> CrosshairWidgetBlueprintArg) { CrosshairWidgetBlueprint = CrosshairWidgetBlueprintArg; }
	FORCEINLINE void SetInGameWidgetBlueprint(TSubclassOf<UUserWidget> InGameWidgetBlueprintArg) { InGameWidgetBlueprint = InGameWidgetBlueprintArg; }

	void UpdateSpectatorWidget(FString Nickname);

	UFUNCTION(Client, Reliable)
	void ClientUpdateInGameWidget();
public:
	UFUNCTION(BlueprintCallable)
	void ShowUICrosshair();
	UFUNCTION(BlueprintCallable)
	void ShowUIInGame();
	UFUNCTION(BlueprintCallable)
	void HideUICrosshair();
	UFUNCTION(BlueprintCallable)
	void HideUIInGame();

private:
	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> CrosshairWidgetBlueprint;
	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> InGameWidgetBlueprint;
	UPROPERTY()
	UUserWidget* InGameWidget;

	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> GameClearWidgetBlueprint;
	UPROPERTY()
	UUserWidget* GameClearWidget;

	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> GameOverWidgetBlueprint;
	UPROPERTY()
	UUserWidget* GameOverWidget;

	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> KnockDownWidgetBlueprint;
	UPROPERTY()
	UUserWidget* KnockDownWidget;

	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> BridgePopupWidgetBlueprint;
	UPROPERTY()
	UUserWidget* BridgePopupWidget;


	UPROPERTY(Replicated)
	TSubclassOf<UUserWidget> SpectatorWidgetBlueprint;
	UPROPERTY()
	UUserWidget* SpectatorWidget;

	UPROPERTY(Replicated)
	class AMEPlayerCharacter* PlayerCharacter;
	

	float CrosshairRadiusWorld;
	float CrosshairRadiusScreen;

	float OldDPIScaling;

	float UIDelay;

	bool InGameUIOnOff;

	float CrosshairHitDelay;
	float CrosshairHitDelayCurrent;

	float CrosshairShootDelayCurrent;
	float CrosshairHeadShotDelayCurrent;

	bool bIsCrosshairShootDelay;
	bool bIsCrosshairHitDelay;
	bool bIsCrosshairHeadShotDelay;

	float GetAttackDelay;
	float GetAttackDelayCurrent;


	// BossUI
public:
	UFUNCTION(Client, Reliable)	
	void ClientAddBossUI(TSubclassOf<UMEYugusUI> BossUIBlueprint);
	
	// Damage Block
public:
	UFUNCTION(Client, Reliable)
	void ClientShowDamageUI(float Damage);

	// Item Type
public:
	UFUNCTION(Client, UnReliable)
		void ClientSetItemNum(int value);
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int GetItemNum() { return ItemNum; }
private:
	int ItemNum;

	bool bIsShowGameClear;
	bool bIsShowGameOver;
	
	TSubclassOf<UUserWidget> DamageUIBlueprint;

private:
	AMEGameStateBase* GameState;
};
