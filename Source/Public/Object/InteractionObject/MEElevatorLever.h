// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "MEElevatorLever.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEElevatorLever : public AMEInteractionObjectBase
{
	GENERATED_BODY()
public:
	AMEElevatorLever();

	virtual void ServerInteraction_Implementation() override;
private:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true), Replicated)
		class AMEElevatorWall* ElevatorWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true), Replicated)
		class AMEElevatorFloor* ElevatorFloor;
};
