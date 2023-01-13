// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectAnimInstance/MEBridgeAnimInstance.h"

UMEBridgeAnimInstance::UMEBridgeAnimInstance()
{

}
void UMEBridgeAnimInstance::PlayBridgeStartAnimation()
{
	Montage_Play(BridgeStartMontage, 1.0f);
}
void UMEBridgeAnimInstance::AnimNotify_StopMontage()
{
	Montage_Pause(GetCurrentActiveMontage());
}
void UMEBridgeAnimInstance::PlayBridgePhase2StopToStartMontage()
{
	Montage_Play(BridgePhase2StopToStartMontage, 1.0f);
}
void UMEBridgeAnimInstance::PlayBridgePhase3StopToStartMontage()
{
	Montage_Play(BridgePhase3StopToStartMontage, 1.0f);
}

void UMEBridgeAnimInstance::AnimNotify_LeverBreakPhase2()
{
	if(LeverBreakPhase2Delegate.IsBound())
		LeverBreakPhase2Delegate.Execute();
}

void UMEBridgeAnimInstance::AnimNotify_LeverBreakPhase3()
{
	if (LeverBreakPhase3Delegate.IsBound())
		LeverBreakPhase3Delegate.Execute();
}

void UMEBridgeAnimInstance::AnimNotify_Landing()
{
	if (LandingDelegate.IsBound())
		LandingDelegate.Execute();
}