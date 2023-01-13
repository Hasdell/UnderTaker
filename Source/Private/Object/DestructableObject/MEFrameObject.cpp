// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MEFrameObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Multiplayer/MEPlayerController.h"

AMEFrameObject::AMEFrameObject()
{
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollision"));
	BodyCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = BodyCollision;

	LeftLegCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftLegCollision"));
	LeftLegCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	LeftLegCollision->SetupAttachment(RootComponent);

	RightLegCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLegCollision"));
	RightLegCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RightLegCollision->SetupAttachment(RootComponent);

	BodyCollision->OnComponentHit.AddDynamic(this, &AMEFrameObject::OnHit);

	DestructibleComponent->SetupAttachment(RootComponent);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}

void AMEFrameObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
	
void AMEFrameObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	if (IsValid(FMODComponent))
		FMODComponent->Play();

	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}

void AMEFrameObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}
float AMEFrameObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();

	return Damage;
}

void AMEFrameObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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