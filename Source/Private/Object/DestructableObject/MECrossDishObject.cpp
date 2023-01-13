// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MECrossDishObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Enemy/Monster/Basscard/MEBasscardCharacter.h"

#include "FMODBlueprintStatics.h"

AMECrossDishObject::AMECrossDishObject()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = BoxCollision;

	DestructibleComponent->SetupAttachment(RootComponent);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}
void AMECrossDishObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}
void AMECrossDishObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
void AMECrossDishObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}

float AMECrossDishObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Cast<AMEBasscardCharacter>(DamageCauser))
	{
		Destruction_Multicast();

		MultiCastToggleActive();
	}

	return Damage;
}