// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEDestructableObjectBase.h"
#include "MEBrokenWoodObject.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEBrokenWoodObject : public AMEDestructableObjectBase
{
	GENERATED_BODY()
public:
	AMEBrokenWoodObject();

	UFUNCTION(NetMulticast, Reliable)
		void Destruction_Multicast();
	UFUNCTION(Server, Reliable)
		void Destruction_Server();

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastToggleActive();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
		class UBoxComponent* BoxCollision = nullptr;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
