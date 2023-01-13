// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEExplosiveObjectBase.h"
#include "MEPowderKeg.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEPowderKeg : public AMEExplosiveObjectBase
{
	GENERATED_BODY()
public:
	AMEPowderKeg();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastToggleActive();
	UFUNCTION(Server, Reliable)
		void ExplosiveHitCheck();

	UFUNCTION(NetMulticast, Reliable)
		void Destruction_Multicast();
	UFUNCTION(Server, Reliable)
		void Destruction_Server();
private:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UCapsuleComponent* Collision = nullptr;
private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Replicated)
		bool bIsDestruction;
};
