// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectAnimInstance/MERewardBoxLidAnimInstance.h"


UMERewardBoxLidAnimInstance::UMERewardBoxLidAnimInstance()
{

}

void UMERewardBoxLidAnimInstance::PlayOpenAnimation()
{
	Montage_Play(OpenAnimMontage, 1.0f);
}
void UMERewardBoxLidAnimInstance::AnimNotify_StopMontage()
{
	Montage_Pause(GetCurrentActiveMontage());
}
void UMERewardBoxLidAnimInstance::AnimNotify_BoxOpenDelegate()
{
	if (BoxOpenDelegate.IsBound())
		BoxOpenDelegate.Execute();
}