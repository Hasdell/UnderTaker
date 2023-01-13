// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEDestructableObjectBase.h"
#include "MECellChairObject.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMECellChairObject : public AMEDestructableObjectBase
{
	GENERATED_BODY()
public:
	AMECellChairObject();
private:
	UFUNCTION(Server, Reliable)
		void Destruction_Server();
	UFUNCTION(NetMulticast, Reliable)
		void Destruction_Multicast();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastToggleActive();
private:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* BoxCollision = nullptr;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
