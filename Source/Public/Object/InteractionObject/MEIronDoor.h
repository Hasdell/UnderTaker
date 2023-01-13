// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "MEIronDoor.generated.h"

UENUM(BlueprintType)
enum class EIronDoorMoveDirection : uint8
{
	Forward = 0,
	Back = 1
};
UCLASS()
class MOONEATER_API AMEIronDoor : public AMEInteractionObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEIronDoor();

	virtual void ServerInteraction_Implementation() override;

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayOpenDoorSound();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = true))
		class UFMODAudioComponent* FMODOpenDoorSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* BridgeCollision = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronDoor", meta = (AllowPrivateAccess = true))
		EIronDoorMoveDirection IronDoorMoveDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronDoor", meta = (AllowPrivateAccess = true))
		float RotationAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronDoor", meta = (AllowPrivateAccess = true))
		float RotationSpeed;

	UPROPERTY(Replicated)
		float Accel;
	UPROPERTY(Replicated)
		bool bIsInteraction;
	UPROPERTY(Replicated)
		FRotator NewRotation;
	UPROPERTY(Replicated)
		float MaxRotatorYaw;
};
