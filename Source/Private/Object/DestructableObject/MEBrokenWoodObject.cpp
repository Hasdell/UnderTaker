// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MEBrokenWoodObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Multiplayer/MEPlayerController.h"

AMEBrokenWoodObject::AMEBrokenWoodObject()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = BoxCollision;

	DestructibleComponent->SetupAttachment(RootComponent);

	BoxCollision->OnComponentHit.AddDynamic(this, &AMEBrokenWoodObject::OnHit);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}
void AMEBrokenWoodObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}
void AMEBrokenWoodObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
void AMEBrokenWoodObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	FMODComponent->Play();

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}

void AMEBrokenWoodObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	TWeakObjectPtr<AMEPlayerCharacter> Player = Cast<AMEPlayerCharacter>(OtherActor);
	if (Player.IsValid())
	{
		Destruction_Server();

		GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
	}
}