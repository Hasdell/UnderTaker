// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DestructableObject/MEDestructableTestObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "DestructibleMesh.h"

#include "Multiplayer/MEPlayerController.h"

AMEDestructableTestObject::AMEDestructableTestObject()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = Sphere;

	DestructibleComponent->SetupAttachment(RootComponent);

	Sphere->OnComponentHit.AddDynamic(this, &AMEDestructableTestObject::OnHit);
}
float AMEDestructableTestObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Destruction_Server();
	
	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
	
	return Damage;
}
void AMEDestructableTestObject::Destruction_Server_Implementation()
{
	Destruction_Multicast();
}
void AMEDestructableTestObject::Destruction_Multicast_Implementation()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);
}
void AMEDestructableTestObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	TWeakObjectPtr<AMEPlayerCharacter> Player = Cast<AMEPlayerCharacter>(OtherActor);
	if (Player.IsValid())
	{
		if (Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::AVOID])
		{
			Destruction_Server();

			GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
		}
	}
}
