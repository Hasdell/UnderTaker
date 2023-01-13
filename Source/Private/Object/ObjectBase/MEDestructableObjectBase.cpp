// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/ObjectBase/MEDestructableObjectBase.h"

#include "DestructibleComponent.h"
#include "Net/UnrealNetwork.h"

#include "FMODBlueprintStatics.h"

AMEDestructableObjectBase::AMEDestructableObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));
	FMODComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMODComponent"));
	FMODComponent->SetAutoActivate(false);
}

void AMEDestructableObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEDestructableObjectBase, DestructibleComponent);
}

// Called when the game starts or when spawned
void AMEDestructableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	DestructibleComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), 1.0);
}

// Called every frame
void AMEDestructableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMEDestructableObjectBase::ServerFadeAway_Implementation()
{
	if (FadeAwayTime != 0.f)
	{
		MulticastStartDeathTick();
		GetWorld()->GetTimerManager().SetTimer(DeathTickTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				MulticastStopDeathTick();
				Destroy();
			}), FadeAwayTime, false);
	}
	else
	{
		Destroy();
	}
}
void AMEDestructableObjectBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (FadeAwayTickerHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(FadeAwayTickerHandle);
		FadeAwayTickerHandle.Reset();
	}
}
void AMEDestructableObjectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}
void AMEDestructableObjectBase::MulticastStartDeathTick_Implementation()
{
	FadeAwayTickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([&](float DeltaTime)
		{
			return DeathTick(DeltaTime);
		}));
}
void AMEDestructableObjectBase::MulticastStopDeathTick_Implementation()
{
	if (FadeAwayTickerHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(FadeAwayTickerHandle);
		FadeAwayTickerHandle.Reset();
	}
}
bool AMEDestructableObjectBase::DeathTick(float DeltaTime)
{
	if (FadeAwayTickerHandle.IsValid())
	{
		CurFadeAwayTime += DeltaTime;
		const float CurTransparency = 1.0f - FMath::Clamp<float>(CurFadeAwayTime / FadeAwayTime, 0.f, 1.0f);
		// DestructibleComponent->SetScalarParameterValueOnMaterials(FName(TEXT("Opacity")), CurTransparency);
		return true;
	}
	return false;
}