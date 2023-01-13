// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectAnimInstance/MEObjectAnimInstance.h"
#include "MEBridgeAnimInstance.generated.h"

/**
 *
 */
DECLARE_DELEGATE(FLeverBreakPhase2Delegate)
DECLARE_DELEGATE(FLeverBreakPhase3Delegate)
DECLARE_DELEGATE(FLandingDelegate)
UCLASS()
class MOONEATER_API UMEBridgeAnimInstance : public UMEObjectAnimInstance
{
	GENERATED_BODY()
public:
	UMEBridgeAnimInstance();

	void PlayBridgeStartAnimation();
	void PlayBridgePhase2StopToStartMontage();
	void PlayBridgePhase3StopToStartMontage();

	UFUNCTION()
		void AnimNotify_StopMontage();
	UFUNCTION()
		void AnimNotify_LeverBreakPhase2();
	UFUNCTION()
		void AnimNotify_LeverBreakPhase3();
	UFUNCTION()
		void AnimNotify_Landing();
public:
	FLeverBreakPhase2Delegate LeverBreakPhase2Delegate;
	FLeverBreakPhase3Delegate LeverBreakPhase3Delegate;
	FLandingDelegate LandingDelegate;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* BridgeStartMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* BridgePhase2StopToStartMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* BridgePhase3StopToStartMontage = nullptr;
};
