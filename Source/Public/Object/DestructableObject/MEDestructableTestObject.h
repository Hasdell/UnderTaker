// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEDestructableObjectBase.h"
#include "MEDestructableTestObject.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEDestructableTestObject : public AMEDestructableObjectBase
{
	GENERATED_BODY()
public:
	AMEDestructableTestObject();
private:
	UFUNCTION(NetMulticast, Reliable)
		void Destruction_Multicast();
	UFUNCTION(Server, Reliable)
		void Destruction_Server();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class USphereComponent* Sphere;
private:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
