// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Multiplayer/MEPlayerController.h"
#include "MEBaseCampController.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEBaseCampController : public AMEPlayerController
{
	GENERATED_BODY()
public:
	AMEBaseCampController();

	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;
public:
	UFUNCTION(Client, Reliable)
		void ClientUpdatePlayerSelectedCharacter();
	UFUNCTION(Server, Reliable)
		void ServerUpdatePlayerSelectedCharacter(TSubclassOf<class AMEPlayerCharacter> NewCharacterBP, TSubclassOf<class AMEPlayerWeapon> NewBaseWeaponBP, TSubclassOf<class UUserWidget> NewCrosshairWidgetBP, TSubclassOf<class UUserWidget> NewInGameWidgetBP);
	UFUNCTION(Client, Reliable)
		void ClientSetBGMComponent();
public:
	UFUNCTION(BlueprintCallable)
		void ChangeCharacter();
public:
	class AMEPlayerCharacter* GetPrevCharacter() const { return PrevCharacter; }
	void SetPrevCharacter(class AMEPlayerCharacter* PrC);
	void PrevCharacterInit(int num);
public:
	void BaseCampSpawnAnna();
	void BaseCampSpawnAlan();
private:
	UFUNCTION(Server, Reliable)
	void ServerBaseCampSpawnAnna();
	UFUNCTION(Server, Reliable)
	void ServerBaseCampSpawnAlan();
private:
	UPROPERTY(Replicated)
	class AMEPlayerCharacter* PrevCharacter;
};
