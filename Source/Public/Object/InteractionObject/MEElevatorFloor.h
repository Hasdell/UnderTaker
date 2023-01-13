// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEElevatorFloor.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEElevatorFloor : public AMEObjectBase
{
	GENERATED_BODY()
public:
	AMEElevatorFloor();

	bool IsAllPlayerGetOnElevator();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastElevatorOperation();

	UFUNCTION(Server, UnReliable)
	void ServerSetActorLocation();
	void ServerSetActorLocation_Implementation() { MultiCastSetActorLocation(); }
	UFUNCTION(NetMulticast, UnReliable)
	void MultiCastSetActorLocation();

	UFUNCTION(Server, Reliable)
	void ServerEntExitCollisionDisable();
	void ServerEntExitCollisionDisable_Implementation() { MultiCastEntExitCollisionDisable(); }
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastEntExitCollisionDisable();

	UFUNCTION()
	void OnTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		EObjectMoveDirection MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EntCollision", meta = (AllowPrivateAccess = true), Replicated)
		class UBoxComponent* EntCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExitCollision", meta = (AllowPrivateAccess = true), Replicated)
		class UBoxComponent* ExitCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (AllowPrivateAccess = true))
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSectionA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
		float ElevatorSectionB;
private:
	UPROPERTY(Replicated)
	TArray<class AMEPlayerCharacter*> PlayerList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorInfo", meta = (AllowPrivateAccess = true))
	float FloorMaxSpeed;
	
	UPROPERTY(Replicated)
	float CurrentSpeed;
	UPROPERTY(Replicated)
	float CurrentTime;

	UPROPERTY(Replicated)
	FVector NewLocation;

	UPROPERTY(Replicated)
	bool bElevatorOperation;
	UPROPERTY(Replicated)
	bool bEnableDecel;
};
