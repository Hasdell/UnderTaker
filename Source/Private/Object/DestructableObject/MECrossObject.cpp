// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MECrossObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Enemy/Monster/Basscard/MEBasscardCharacter.h"

AMECrossObject::AMECrossObject()
{
	RowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RowCollision"));
	RowCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = RowCollision;

	ColumnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ColumnCollision"));
	ColumnCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	ColumnCollision->SetupAttachment(RootComponent);

	DestructibleComponent->SetupAttachment(RootComponent);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	Cloth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClothMesh"));
	Cloth->SetCollisionProfileName(TEXT("NoCollision"));
	Cloth->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}
float AMECrossObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Cast<AMEBasscardCharacter>(DamageCauser))
	{
		Destruction_Multicast();

		MultiCastToggleActive();
	}
	return Damage;
}

void AMECrossObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();
}

void AMECrossObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	RowCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ColumnCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cloth->SetHiddenInGame(true);

	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 1000.f);
}

void AMECrossObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}