// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEElevatorWall.generated.h"

/**
 * 
 */

UCLASS()
class MOONEATER_API AMEElevatorWall : public AMEObjectBase
{
	GENERATED_BODY()
		
public:
	AMEElevatorWall();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastElevatorOperation();

	UFUNCTION(Server, UnReliable)
	void ServerSetActorLocation();
	void ServerSetActorLocation_Implementation() { MultiCastSetActorLocation(); }
	UFUNCTION(NetMulticast, UnReliable)
	void MultiCastSetActorLocation();
private:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		EObjectMoveDirection MoveDirection;

	// ElevatorStartDustEft 동작 이펙트
	// ElevatorStartSound 동작 사운드
	// ElevatorArrivalSound 도착 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSpeedA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSpeedB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSpeedC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSectionA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSectionB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSectionC;

	UPROPERTY(Replicated)
	float CurrentElevatorSpeed;
	UPROPERTY(Replicated)
	float CurrentTime;

	UPROPERTY(Replicated)
	bool bElevatorOperation;
	UPROPERTY(Replicated)
	bool bEnableAccel;
	UPROPERTY(Replicated)
	bool bEnableDecel;

	UPROPERTY(Replicated)
	FVector NewLocation;
};
