// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEIronBar.generated.h"

UCLASS()
class MOONEATER_API AMEIronBar : public AMEObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEIronBar();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastIronBarOperaction();
	UFUNCTION(NetMulticast, UnReliable)
	void MultiCastSetActorLocation();

	UFUNCTION(Server, UnReliable)
		void ServerIronBarStop();

	UFUNCTION(Server, UnReliable)
	void ServerSetMoveDirection(EObjectMoveDirection MoveDirection);
	UFUNCTION(NetMulticast, UnReliable)
	void MultiCastSetMoveDirection(EObjectMoveDirection MoveDirection);

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayStopSound();

	bool GetIsOperaction();
	EIronBarOperationMode GetOperationMode();
	EObjectMoveDirection GetMoveDirection();
	int	GetProgress();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = true))
		class UFMODAudioComponent* FMODOperationSoundComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = true))
		class UFMODAudioComponent* FMODStopSoundComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		EIronBarOperationMode OperationMode;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true), Replicated)
		EObjectMoveDirection IronBarMoveDirection;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		float Speed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		float MaxMoveDistance;

	UPROPERTY(Replicated)
		bool bIsIronBarOperaction;
	UPROPERTY(Replicated)
		FVector NewLocation;
	UPROPERTY(Replicated)
		float Accel;

	UPROPERTY(Replicated)
		float MaxMoveZVector;

	UPROPERTY(Replicated)
		int Progress;

	float ProgressTimer;
};
