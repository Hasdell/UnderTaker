// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectAnimInstance/MEObjectAnimInstance.h"
#include "MERewardBoxBodyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API UMERewardBoxBodyAnimInstance : public UMEObjectAnimInstance
{
	GENERATED_BODY()
public:
	UMERewardBoxBodyAnimInstance();

	void PlayOpenAnimation();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = true))
		UAnimMontage* OpenAnimMontage = nullptr;
};
