// Fill out your copyright notice in the Description page of Project Settings.



#include "Object/ExplosiveObject/MEPowderKeg.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Shared/Base/MESharedBuffComponent.h"

#include "Object/DestructableObject/MEBarrelObject.h"

#include "Net/UnrealNetwork.h"

#include "Multiplayer/MEPlayerController.h"

AMEPowderKeg::AMEPowderKeg()
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComponent"));
	Collision->SetCollisionProfileName(TEXT("DestructableObject"));
	RootComponent = Collision;

	DestructibleComponent->SetupAttachment(RootComponent);

	Collision->OnComponentHit.AddDynamic(this, &AMEPowderKeg::OnHit);

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ExplosionEffect->SetupAttachment(RootComponent);

	FMODComponent->SetupAttachment(RootComponent);

	ExplosionEffect->bAutoActivate = false;
}
void AMEPowderKeg::ExplosiveHitCheck_Implementation()
{
	bIsDestruction = true;

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		GetActorLocation(),
		ExplosionRadius,
		nullptr,
		TArray<AActor*>(),
		this,
		false
	);

	UWorld* World = GetWorld();

	TArray<FOverlapResult> OverlapResults;
	TArray<AActor*> IgnoreActors;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);

	FVector Box = FVector(ExplosionRadius, ExplosionRadius, ExplosionRadius);

	bool bResult = World->OverlapMultiByChannel( // 지정된 Collision FCollisionShape와 충돌한 액터 감지 
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeBox(Box),
		CollisionQueryParam
	);
	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AActor* Target = OverlapResult.GetActor();

			if (OverlapResult.GetActor() == this)
				continue;

			if (Cast<AMEPowderKeg>(OverlapResult.GetActor()))
			{
				AMEPowderKeg* PowderKeg = Cast<AMEPowderKeg>(OverlapResult.GetActor());

				if (!PowderKeg->bIsDestruction)
				{
					PowderKeg->Destruction_Server();
					PowderKeg->ExplosiveHitCheck();
				}

				continue;
			}
			else if (Cast<AMEBarrelObject>(OverlapResult.GetActor()))
			{
				AMEBarrelObject* Barrel = Cast<AMEBarrelObject>(OverlapResult.GetActor());

				Barrel->Destruction_Server();

				continue;
			}

			UMESharedBuffComponent* BuffComponent
				= Cast<UMESharedBuffComponent>(Target->GetComponentByClass(UMESharedBuffComponent::StaticClass()));

			if (BuffComponent != nullptr)
			{
				BuffComponent->SetBuffDebuff(EBuffDebuffEnum::PowderKegDebuff, this);
			}
		}
	}
}

float AMEPowderKeg::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!Cast<AMEPowderKeg>(DamageCauser))
	{
		ExplosiveHitCheck_Implementation();

		Destruction_Server();
	}

	return Damage;
}
void AMEPowderKeg::MultiCastToggleActive_Implementation()
{
	ExplosionEffect->ToggleActive();
}
void AMEPowderKeg::Destruction_Server_Implementation()
{
	Destruction_Multicast();

	MultiCastToggleActive();
}
void AMEPowderKeg::Destruction_Multicast_Implementation()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructibleComponent->ApplyDamage(20, GetActorLocation(), FVector(0.f, 0.f, -1.f), 5000.f);

	if (IsValid(FMODComponent))
		FMODComponent->Play();

	GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
}
void AMEPowderKeg::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEPowderKeg, bIsDestruction);
}
void AMEPowderKeg::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	TWeakObjectPtr<AMEPlayerCharacter> Player = Cast<AMEPlayerCharacter>(OtherActor);
	if (Player.IsValid())
	{
		if (Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::AVOID])
		{
			Destruction_Multicast();

			GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
		}
		if (Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_ENTER] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_LOOP_LOW] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_LOOP_HIGH] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_EXIT] ||
			Player->GetStateManager()->GetCurrentStateUpper() == Player->GetStateManager()->UpperState[(uint8)EPlayerUpperState::JUMP_EXIT_HIGH])
		{
			Destruction_Multicast();

			GetWorld()->GetTimerManager().SetTimer(FadeAwayTimerHandle, this, &AMEDestructableObjectBase::ServerFadeAway, FadeAwayWaitingTime, false);
		}
	}
}