// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectAnimInstance/MEObjectAnimInstance.h"
#include "MELeverAnimInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FLeverUpDelegate)
DECLARE_DELEGATE(FLeverDownDelegate)
UCLASS()
class MOONEATER_API UMELeverAnimInstance : public UMEObjectAnimInstance
{
	GENERATED_BODY()
public:
	UMELeverAnimInstance();

	void PlayLeverDownMontage();
	void PlayLeverUpMontage();
private:
	UFUNCTION()
		void AnimNotify_StopMontage();
	UFUNCTION()
		void AnimNotify_LeverDownEvent();
	UFUNCTION()
		void AnimNotify_LeverUpEvent();
public:
	FLeverUpDelegate LeverUpDelegate;
	FLeverDownDelegate LeverDownDelegate;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* LeverUpMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* LeverDownMontage = nullptr;
};
