// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MEBarrelObject.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DestructibleMesh.h"

#include "Multiplayer/MEPlayerController.h"

AMEBarrelObject::AMEBarrelObject()
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComponent"));
	Collision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = Collision;

	DestructibleComponent->SetupAttachment(RootComponent);

	Collision->OnComponentHit.AddDynamic(this, &AMEBarrelObject::OnHit);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	DestructionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	DestructionEffect->bAutoActivate = false;
}
float AMEBarrelObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();

	return Damage;
}
void AMEBarrelObject::MultiCastToggleActive_Implementation()
{
	DestructionEffect->ToggleActive();
}
void AMEBarrelObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
void AMEBarrelObject::Destruction_Multicast_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);

	if(IsValid(FMODComponent))
		FMODComponent->Play();

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}
void AMEBarrelObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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