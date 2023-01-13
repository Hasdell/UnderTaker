// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectAnimInstance/MERewardBoxBodyAnimInstance.h"

UMERewardBoxBodyAnimInstance::UMERewardBoxBodyAnimInstance()
{

}

void UMERewardBoxBodyAnimInstance::PlayOpenAnimation()
{
	Montage_Play(OpenAnimMontage, 1.0f);
}