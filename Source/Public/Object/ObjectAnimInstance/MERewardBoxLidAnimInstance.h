// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectAnimInstance/MEObjectAnimInstance.h"
#include "MERewardBoxLidAnimInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FBoxOpenDelegate)
UCLASS()
class MOONEATER_API UMERewardBoxLidAnimInstance : public UMEObjectAnimInstance
{
	GENERATED_BODY()
public:
	UMERewardBoxLidAnimInstance();

	void PlayOpenAnimation();

	UFUNCTION()
	void AnimNotify_StopMontage();
	UFUNCTION()
		void AnimNotify_BoxOpenDelegate();
public:
	FBoxOpenDelegate BoxOpenDelegate;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
	UAnimMontage* OpenAnimMontage = nullptr;
};
