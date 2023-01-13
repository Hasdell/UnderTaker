// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "MEBridgeLever.generated.h"

UCLASS()
class MOONEATER_API AMEBridgeLever : public AMEInteractionObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEBridgeLever();

	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastActiveBreakEffect();
	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastDisableBreakEffect();

	virtual void ServerInteraction_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
		void BreakEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void RepairEvent();
private:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION(Server, UnReliable)
		void ServerStartPhase2();
		void ServerStartPhase2_Implementation() { MultiCastStartPhase2(); }
	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastStartPhase2();

	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastPlayLeverUpAnim();
	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastPlayLeverDownAnim();

	UFUNCTION(Server, Reliable)
		void ServerSetLeverAnimInstance();
		void ServerSetLeverAnimInstance_Implementation() { MultiCastSetLeverAnimInstance(); }
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastSetLeverAnimInstance();

	UFUNCTION(Server, Reliable)
		void ServerLeverUpEvent();
	UFUNCTION(Server, Reliable)
		void ServerLeverDownEvent();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastBreakEvent();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastRepairEvent();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* LeverMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance", Meta = (AllowPrivateAccess = true))
		class UMELeverAnimInstance* LeverAnimInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* BreakEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge", meta = (AllowPrivateAccess = true))
	class AMEBridge* Bridge = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge", meta = (AllowPrivateAccess = true))
	int Phase2Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge", meta = (AllowPrivateAccess = true))
	int Phase3Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase1Trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase2Trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase3Trigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge", meta = (AllowPrivateAccess = true))
		float Phase3EndTime;

	UPROPERTY(Replicated)
	bool bIsPhase2Repair;
	UPROPERTY(Replicated)
	bool bIsPhase3Repair;

	UPROPERTY(Replicated)
	bool bIsPhase1Start;
	UPROPERTY(Replicated)
	bool bIsPhase2Start;
	UPROPERTY(Replicated)
	bool bIsPhase3Start;

	UPROPERTY(Replicated)
	bool bIsInteraction;

	UPROPERTY(Replicated)
	float BridgeDownTimer;

	UPROPERTY(Replicated)
		float Phase3EndTimer;

	// LeverRotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* InteractionCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeLever", meta = (AllowPrivateAccess = true))
		float LeverRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeLever", meta = (AllowPrivateAccess = true))
		float OperationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeLever", meta = (AllowPrivateAccess = true))
		float LeverBreakRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeLever", meta = (AllowPrivateAccess = true))
		float LeverBreakOperationTime;

	UPROPERTY(Replicated)
	bool bIsLeverOperationCompleted;
	UPROPERTY(Replicated)
	bool bIsLeverOperation;
	UPROPERTY(Replicated)
	bool bIsLeverBreak;

	UPROPERTY(Replicated)
	FRotator NewRotator;
	
	UPROPERTY(Replicated)
	float OperationTimer;
	//
};
