// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectBase/MEObjectBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Multiplayer/MEGameModeBase.h"

// Sets default values
AMEObjectBase::AMEObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);
}

void AMEObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void AMEObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<AMEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AMEObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

