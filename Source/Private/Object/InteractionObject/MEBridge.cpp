// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MEBridge.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Net/UnrealNetwork.h"

#include "Multiplayer/MEGameModeBase.h"

#include "Object/ObjectAnimInstance/MEBridgeAnimInstance.h"

AMEBridge::AMEBridge()
{
	BridgeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	BridgeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BridgeMesh;

	DustEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DustEffect->SetupAttachment(RootComponent);

	DustEffect->bAutoActivate = false;
}
void AMEBridge::MultiCastSetAnimInstance_Implementation()
{
	BridgeAnimInstance = Cast<UMEBridgeAnimInstance>(BridgeMesh->GetAnimInstance());
}
void AMEBridge::ServerBridgeDown_Implementation()
{
	bDown = true;

	if (Progress >= 100.f)
	{
		ServerBridgeLading();
	}
	else
	{
		Progress++;
	}
}
void AMEBridge::ServerBridgeStop_Implementation()
{
	bDown = false;
}
void AMEBridge::ServerBridgeLading_Implementation()
{
	bLanding = true;
}
void AMEBridge::ServerSetLandingDelegate_Implementation()
{
	BridgeAnimInstance->LandingDelegate.BindUFunction(this, FName("ServerBridgeLading"));
}
float AMEBridge::GetProgress()
{
	return Progress;
}
bool AMEBridge::GetIsBridgeLanding()
{
	return IsBridgeLanding;
}
bool AMEBridge::GetLandingTestMode()
{
	return LandingTestMode;
}
UMEBridgeAnimInstance* AMEBridge::GetBridgeAnimInstance()
{
	return BridgeAnimInstance;
}
void AMEBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLanding)
	{
		Landing();
	}
}
void AMEBridge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEBridge, Progress);

	DOREPLIFETIME(AMEBridge, bLanding);
	DOREPLIFETIME(AMEBridge, bDown);

	DOREPLIFETIME(AMEBridge, NewRotator);

	DOREPLIFETIME(AMEBridge, IsBridgeLanding);

	DOREPLIFETIME(AMEBridge, LandingTimer);
}
void AMEBridge::Landing()
{
	bDown = false;
	bLanding = false;
	IsBridgeLanding = true;

	MultiCastActiveEffect();

	ServerSetBlockingActorActive();
	MultiCastCameraShake();
	SetActorTickEnabled(false);
}
void AMEBridge::MultiCastCameraShake_Implementation()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (IsValid(Controller) && GetNetMode() == ENetMode::NM_Client && IsValid(BridgeLandingCameraShake))
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(BridgeLandingCameraShake);
	}
}
void AMEBridge::ServerSetBlockingActorActive_Implementation()
{
	MultiCastSetBlockingActorActive();
}
void AMEBridge::MultiCastSetBlockingActorActive_Implementation()
{
	if (IsValid(BlockingActor))
	{
		BlockingActor->SetActorEnableCollision(bActorActive);
		BlockingActor->SetActorHiddenInGame(!bActorActive);
	}
}

void AMEBridge::MultiCastActiveEffect_Implementation()
{
	DustEffect->ToggleActive();
}

void AMEBridge::MultiCastPlayStartAnim_Implementation()
{
	BridgeAnimInstance->PlayBridgeStartAnimation();
}

void AMEBridge::MultiCastPlayPhase2StopToStartAnim_Implementation()
{
	BridgeAnimInstance->PlayBridgePhase2StopToStartMontage();
}

void AMEBridge::MultiCastPlayPhase3StopToStartAnim_Implementation()
{
	BridgeAnimInstance->PlayBridgePhase3StopToStartMontage();
}