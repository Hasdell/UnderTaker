// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectAnimInstance/MELeverAnimInstance.h"

UMELeverAnimInstance::UMELeverAnimInstance()
{

}
void UMELeverAnimInstance::PlayLeverDownMontage()
{
	Montage_Play(LeverDownMontage, 1.0f);
}
void UMELeverAnimInstance::PlayLeverUpMontage()
{
	Montage_Play(LeverUpMontage, 1.0f);
}
void UMELeverAnimInstance::AnimNotify_StopMontage()
{
	Montage_Pause(GetCurrentActiveMontage());
}
void UMELeverAnimInstance::AnimNotify_LeverDownEvent()
{
	if (LeverDownDelegate.IsBound())
		LeverDownDelegate.Execute();
}
void UMELeverAnimInstance::AnimNotify_LeverUpEvent()
{
	if (LeverUpDelegate.IsBound())
		LeverUpDelegate.Execute();
}