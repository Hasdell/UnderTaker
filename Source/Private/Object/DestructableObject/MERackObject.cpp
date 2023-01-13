// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MERackObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Multiplayer/MEPlayerController.h"

AMERackObject::AMERackObject()
{
	RightLegCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLegCollision"));
	RightLegCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = RightLegCollision;

	CrossBodyLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CrossBodyLeftCollision"));
	CrossBodyLeftCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	CrossBodyLeftCollision->SetupAttachment(RootComponent);
	CrossBodyRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CrossBodyRightCollision"));
	CrossBodyRightCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	CrossBodyRightCollision->SetupAttachment(RootComponent);
	LeftLegCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftLegCollision"));
	LeftLegCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	LeftLegCollision->SetupAttachment(RootComponent);

	DestructibleComponent->SetupAttachment(RootComponent);

	CrossBodyLeftCollision->OnComponentHit.AddDynamic(this, &AMERackObject::OnHit);
	CrossBodyRightCollision->OnComponentHit.AddDynamic(this, &AMERackObject::OnHit);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}

void AMERackObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}

void AMERackObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	if (IsValid(FMODComponent))
		FMODComponent->Play();

	CrossBodyLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CrossBodyRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}


void AMERackObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}

float AMERackObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();

	return Damage;
}

void AMERackObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	TWeakObjectPtr<AMEPlayerCharacter> Player = Cast<AMEPlayerCharacter>(OtherActor);
	if (Player.IsValid())
	{
		if (Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::AVOID])
		{
			Destruction_Server();

			GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
		}
		if (Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_ENTER] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_LOOP_LOW] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_LOOP_HIGH] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_EXIT] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_EXIT_HIGH])
		{
			Destruction_Server();

			GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
		}
	}
}