// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "MEIronBarLever.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEIronBarLever : public AMEInteractionObjectBase
{
	GENERATED_BODY()
public:
	AMEIronBarLever();

	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastActiveBreakEffect();
	UFUNCTION(NetMulticast, UnReliable)
		void MultiCastDisableBreakEffect();

	virtual void ServerInteraction_Implementation() override;

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

	UFUNCTION(BlueprintImplementableEvent)
		void BreakEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void RepairEvent();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastBreakEvent();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastRepairEvent();
private:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* LeverMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance", Meta = (AllowPrivateAccess = true))
		class UMELeverAnimInstance* LeverAnimInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* BreakEffect = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		class AMEIronBar* IronBar = nullptr;

	//	Defense
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		int Phase2Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		int Phase3Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase1Trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase2Trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnTrigger", meta = (AllowPrivateAccess = true))
		class AMEMonsterTrigger* Phase3Trigger = nullptr;


	UPROPERTY(Replicated)
		bool bIsPhase1Start;
	UPROPERTY(Replicated)
		bool bIsPhase2Start;

	UPROPERTY(Replicated)
		bool bIsPhase2Repair;
	
	UPROPERTY(Replicated)
		bool bIsInteraction;

	UPROPERTY(Replicated)
		float IronBarDownTimer;
	//

	// LeverRotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* InteractionCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBarLever", meta = (AllowPrivateAccess = true))
		float LeverRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBarLever", meta = (AllowPrivateAccess = true))
		float OperationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBarLever", meta = (AllowPrivateAccess = true))
		float LeverBreakRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBarLever", meta = (AllowPrivateAccess = true))
		float LeverBreakOperationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IronBarLever", meta = (AllowPrivateAccess = true))
		float Phase3EndTime;

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

	UPROPERTY(Replicated)
		float Phase3EndTimer;
	//
};
