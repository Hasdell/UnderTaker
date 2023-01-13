// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MECellChairObject.h"

#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Multiplayer/MEPlayerController.h"

AMECellChairObject::AMECellChairObject()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = BoxCollision;

	DestructibleComponent->SetupAttachment(RootComponent);

	BoxCollision->OnComponentHit.AddDynamic(this, &AMECellChairObject::OnHit);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}
void AMECellChairObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}
void AMECellChairObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
void AMECellChairObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	if (IsValid(FMODComponent))
		FMODComponent->Play();

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}

float AMECellChairObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();

	return Damage;
}

void AMECellChairObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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