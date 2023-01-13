// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEBridgeLever.h"
#include "Object/InteractionObject/MEBridge.h"
#include "Object/ObjectAnimInstance/MEBridgeAnimInstance.h"

#include "Net/UnrealNetwork.h"

#include "Enemy/Monster/Other/MEMonsterTrigger.h"

#include "Components/WidgetComponent.h"

#include "Object/ObjectAnimInstance/MELeverAnimInstance.h"

// Sets default values
AMEBridgeLever::AMEBridgeLever()
{
	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeverMeshComponent"));
	LeverMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = LeverMesh;

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	InteractionCollision->SetupAttachment(LeverMesh);

	BreakEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	BreakEffect->SetupAttachment(RootComponent);

	InteractionWidgetComponent->SetupAttachment(RootComponent);
	MarkWidgetComponent->SetupAttachment(RootComponent);

	BreakEffect->bAutoActivate = false;
}
void AMEBridgeLever::ServerInteraction_Implementation()
{
	if (bCanInteraction)
	{
		if (Bridge->GetIsBridgeLanding())
			return;

		bCanInteraction = false;

		if (!IsValid(LeverAnimInstance))
		{
			ServerSetLeverAnimInstance();

			LeverAnimInstance->LeverDownDelegate.BindUFunction(this, FName("ServerLeverDownEvent"));
			LeverAnimInstance->LeverUpDelegate.BindUFunction(this, FName("ServerLeverUpEvent"));
		}
		MultiCastPlayLeverDownAnim();

		if (bIsPhase2Start)
		{
			MultiCastDisableBreakEffect();
		}

		MultiCastRepairEvent();
	}
}
// Called every frame
void AMEBridgeLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Bridge) return;

	if (Bridge->GetIsBridgeLanding())
	{
		bCanInteraction = false;

		Phase3EndTimer += DeltaTime;
		if (Phase3EndTimer >= Phase3EndTime)
		{
			SetActorTickEnabled(false);

			Phase3Trigger->OnDisableTrigger();
		}
		return;
	}


	if (bIsInteraction)
	{
		if (Bridge->GetIsBridgeLanding())
		{
			bIsInteraction = false;

			Phase2Trigger->OnDisableTrigger();
			Phase3Trigger->OnEnableTrigger();
		}

		if (!bIsPhase1Start)
		{
			// Phase1
			Phase1Trigger->OnEnableTrigger();

			bIsPhase1Start = true;
		}
	}
}
void AMEBridgeLever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEBridgeLever, bIsInteraction);

	DOREPLIFETIME(AMEBridgeLever, BridgeDownTimer);

	DOREPLIFETIME(AMEBridgeLever, bIsPhase2Repair);

	DOREPLIFETIME(AMEBridgeLever, bIsPhase1Start);
	DOREPLIFETIME(AMEBridgeLever, bIsPhase2Start);

	DOREPLIFETIME(AMEBridgeLever, Phase3EndTimer);

	DOREPLIFETIME(AMEBridgeLever, bIsLeverOperationCompleted);
	DOREPLIFETIME(AMEBridgeLever, bIsLeverOperation);
	DOREPLIFETIME(AMEBridgeLever, bIsLeverBreak);

	DOREPLIFETIME(AMEBridgeLever, NewRotator);

	DOREPLIFETIME(AMEBridgeLever, OperationTimer);
}
void AMEBridgeLever::MultiCastActiveBreakEffect_Implementation()
{
	BreakEffect->ToggleActive();
}

void AMEBridgeLever::MultiCastDisableBreakEffect_Implementation()
{
	BreakEffect->ToggleActive();
}

void AMEBridgeLever::MultiCastStartPhase2_Implementation()
{
	MultiCastPlayLeverUpAnim();
}

void AMEBridgeLever::MultiCastSetLeverAnimInstance_Implementation()
{
	LeverAnimInstance = Cast<UMELeverAnimInstance>(LeverMesh->GetAnimInstance());
}

void AMEBridgeLever::MultiCastPlayLeverUpAnim_Implementation()
{
	LeverAnimInstance->PlayLeverUpMontage();
}

void AMEBridgeLever::MultiCastPlayLeverDownAnim_Implementation()
{
	LeverAnimInstance->PlayLeverDownMontage();
}

void AMEBridgeLever::ServerLeverDownEvent_Implementation()
{
	if (bIsPhase2Start && !bIsPhase2Repair)
	{
		bIsPhase2Repair = true;

		if (IsValid(Bridge))
		{
			Bridge->MultiCastPlayPhase2StopToStartAnim();
		}
	}
	else
	{
		bIsInteraction = true;

		if (IsValid(Bridge))
		{
			Bridge->ServerSetBridgeAnimInstance();

			Bridge->GetBridgeAnimInstance()->LeverBreakPhase2Delegate.BindUFunction(this, FName("ServerStartPhase2"));
			Bridge->GetBridgeAnimInstance()->LandingDelegate.BindUFunction(Bridge, FName("ServerBridgeLading"));

			if (Bridge->GetLandingTestMode())
				Bridge->MultiCastPlayPhase2StopToStartAnim();
			else
				Bridge->MultiCastPlayStartAnim();
		}
	}
}
void AMEBridgeLever::ServerLeverUpEvent_Implementation()
{
	if (!bIsPhase2Start)
	{
		Phase1Trigger->OnDisableTrigger();
		Phase2Trigger->OnEnableTrigger();

		bIsPhase2Start = true;

		MultiCastBreakEvent();
	}

	bCanInteraction = true;
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
		MultiCastActiveBreakEffect();
}

void AMEBridgeLever::MultiCastBreakEvent_Implementation()
{
	BreakEvent();
}
void AMEBridgeLever::MultiCastRepairEvent_Implementation()
{
	RepairEvent();
}