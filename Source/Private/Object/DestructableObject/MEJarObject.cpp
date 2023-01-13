// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MEJarObject.h"

#include "Components/CapsuleComponent.h"

#include "Multiplayer/MEPlayerController.h"

#include "Multiplayer/MEPlayerController.h"

AMEJarObject::AMEJarObject()
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Collision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = Collision;

	DestructibleComponent->SetupAttachment(RootComponent);

	Collision->OnComponentHit.AddDynamic(this, &AMEJarObject::OnHit);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}

void AMEJarObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
	
void AMEJarObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	if (IsValid(FMODComponent))
		FMODComponent->Play();

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}


void AMEJarObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}

float AMEJarObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();

	return Damage;
}

void AMEJarObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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