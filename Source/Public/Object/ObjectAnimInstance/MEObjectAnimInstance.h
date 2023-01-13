// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MEObjectAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API UMEObjectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMEObjectAnimInstance();

	virtual	void NativeUpdateAnimation(float DeltaSeconds) override;
};
