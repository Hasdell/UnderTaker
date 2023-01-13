// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEBridge.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEBridge : public AMEObjectBase
{
	GENERATED_BODY()
public:
	AMEBridge();

	UFUNCTION(Server, Reliable)
	void ServerBridgeDown();
	UFUNCTION(Server, Reliable)
	void ServerBridgeStop();
	UFUNCTION(Server, Reliable)
	void ServerBridgeLading();

	UFUNCTION(Server, Reliable)
		void ServerSetBridgeAnimInstance();

	UFUNCTION(Server, Reliable)
		void ServerSetLandingDelegate();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayPhase2StopToStartAnim();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayPhase3StopToStartAnim();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayStartAnim();

	float GetProgress();
	bool GetIsBridgeLanding();
	bool GetLandingTestMode();
	class UMEBridgeAnimInstance* GetBridgeAnimInstance();
private:
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastCameraShake();

	UFUNCTION(Server, Reliable)
		void ServerSetBlockingActorActive();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastSetBlockingActorActive();

		void ServerSetBridgeAnimInstance_Implementation() { MultiCastSetAnimInstance(); }
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastSetAnimInstance();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastActiveEffect();


	void Landing();
private:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* BridgeCollision = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "AnimInstance")
		class UMEBridgeAnimInstance* BridgeAnimInstance = nullptr;
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraShake, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class UCameraShakeBase> BridgeLandingCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* BridgeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* DustEffect = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BridgeInfo", meta = (AllowPrivateAccess = true), Replicated)
		float Progress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeInfo", meta = (AllowPrivateAccess = true))
		float BridgeDownSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BridgeInfo", meta = (AllowPrivateAccess = true))
		float LandingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		bool LandingTestMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = BlockingActor)
		AActor* BlockingActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = BlockingActor)
		bool bActorActive{ false };

	UPROPERTY(Replicated)
		bool bLanding;
	UPROPERTY(Replicated)
		bool bDown;

	UPROPERTY(Replicated)
		FRotator NewRotator;

	UPROPERTY(Replicated)
		bool IsBridgeLanding;

	UPROPERTY(Replicated)
		float LandingTimer;
};
