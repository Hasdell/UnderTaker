// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEIronBarLever.h"

#include "Object/InteractionObject/MEIronBar.h"

#include "Net/UnrealNetwork.h"

#include "Enemy/Monster/Other/MEMonsterTrigger.h"

#include "Components/WidgetComponent.h"

#include "Object/ObjectAnimInstance/MELeverAnimInstance.h"

AMEIronBarLever::AMEIronBarLever()
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

void AMEIronBarLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(IronBar))
		return;

	if (!bIsLeverOperation)
		return;

	if (IronBar->GetOperationMode() == EIronBarOperationMode::Defense)
	{
		if (IronBar->GetProgress() >= 100)
		{
			Phase3EndTimer += DeltaTime;
			if (Phase3EndTimer >= Phase3EndTime)
			{
				SetActorTickEnabled(false);

				Phase3Trigger->OnDisableTrigger();
			}
			return;
		}

		if (!bIsPhase1Start)
		{
			// Phase1
			Phase1Trigger->OnEnableTrigger();

			bIsPhase1Start = true;
		}
		if (IronBar->GetProgress() >= Phase2Start && !bIsPhase2Repair && !bIsPhase2Start)
		{
			// Phase2
			MultiCastPlayLeverUpAnim();

			if(GetNetMode() == ENetMode::NM_DedicatedServer)
				MultiCastActiveBreakEffect();

			IronBar->ServerIronBarStop();

			bIsPhase2Start = true;

			bCanInteraction = false;

			return;
		}

		if (bIsPhase2Start && !bIsPhase2Repair)
			return;

		if (IronBar->GetProgress() >= 100)
		{
			Phase3Trigger->OnEnableTrigger();
		}
	}
}
void AMEIronBarLever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEIronBarLever, bIsPhase1Start);
	DOREPLIFETIME(AMEIronBarLever, bIsPhase2Start);

	DOREPLIFETIME(AMEIronBarLever, bIsPhase2Repair);

	DOREPLIFETIME(AMEIronBarLever, bIsInteraction);
	DOREPLIFETIME(AMEIronBarLever, IronBarDownTimer);

	DOREPLIFETIME(AMEIronBarLever, bIsLeverOperationCompleted);
	DOREPLIFETIME(AMEIronBarLever, bIsLeverOperation);
	DOREPLIFETIME(AMEIronBarLever, bIsLeverBreak);

	DOREPLIFETIME(AMEIronBarLever, NewRotator);

	DOREPLIFETIME(AMEIronBarLever, OperationTimer);

	DOREPLIFETIME(AMEIronBarLever, Phase3EndTimer);
}
void AMEIronBarLever::ServerInteraction_Implementation()
{
	if (bCanInteraction)
	{
		bCanInteraction = false;

		if (!IsValid(LeverAnimInstance))
		{
			ServerSetLeverAnimInstance();

			LeverAnimInstance->LeverUpDelegate.BindUFunction(this, FName("ServerLeverUpEvent"));
			LeverAnimInstance->LeverDownDelegate.BindUFunction(this, FName("ServerLeverDownEvent"));
		}

		MultiCastPlayLeverDownAnim();

		if (!IsValid(IronBar))
			return;

		if (IronBar->GetOperationMode() == EIronBarOperationMode::Defense)
		{
			if (bIsPhase2Start)
			{
				MultiCastDisableBreakEffect();
			}
			MultiCastRepairEvent();
		}
	}
}

void AMEIronBarLever::MultiCastActiveBreakEffect_Implementation()
{
	BreakEffect->ToggleActive();
}

void AMEIronBarLever::MultiCastDisableBreakEffect_Implementation()
{
	BreakEffect->ToggleActive();
}

void AMEIronBarLever::MultiCastPlayLeverUpAnim_Implementation()
{
	LeverAnimInstance->PlayLeverUpMontage();
}
void AMEIronBarLever::MultiCastPlayLeverDownAnim_Implementation()
{
	LeverAnimInstance->PlayLeverDownMontage();
}

void AMEIronBarLever::MultiCastSetLeverAnimInstance_Implementation()
{
	LeverAnimInstance = Cast<UMELeverAnimInstance>(LeverMesh->GetAnimInstance());
}


void AMEIronBarLever::ServerLeverUpEvent_Implementation()
{
	if (IronBar->GetOperationMode() == EIronBarOperationMode::Normal)
	{
		bCanInteraction = true;
	}
	if (IronBar->GetOperationMode() == EIronBarOperationMode::Defense)
	{
		Phase1Trigger->OnDisableTrigger();
		Phase2Trigger->OnEnableTrigger();

		MultiCastBreakEvent();

		bCanInteraction = true;
	}
	bIsLeverOperation = false;
}

void AMEIronBarLever::ServerLeverDownEvent_Implementation()
{
	if (IronBar->GetOperationMode() == EIronBarOperationMode::Normal)
	{
		IronBar->MultiCastIronBarOperaction();
		MultiCastPlayLeverUpAnim();
	}
	else if (IronBar->GetOperationMode() == EIronBarOperationMode::Defense)
	{
		IronBar->MultiCastIronBarOperaction();

		if (bIsPhase2Start && !bIsPhase2Repair)
			bIsPhase2Repair = true;
	}
	bIsLeverOperation = true;
}

void AMEIronBarLever::MultiCastBreakEvent_Implementation()
{
	BreakEvent();
}
void AMEIronBarLever::MultiCastRepairEvent_Implementation()
{
	RepairEvent();
}