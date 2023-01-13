// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEDestructableObjectBase.h"
#include "MEBarrelObject.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEBarrelObject : public AMEDestructableObjectBase
{
	GENERATED_BODY()
public:
	AMEBarrelObject();

	UFUNCTION(NetMulticast, Reliable)
		void Destruction_Multicast();
	UFUNCTION(Server, Reliable)
		void Destruction_Server();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastToggleActive();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UCapsuleComponent* Collision;

private:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
