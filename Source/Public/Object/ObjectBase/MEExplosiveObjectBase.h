// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEDestructableObjectBase.h"
#include "MEExplosiveObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEExplosiveObjectBase : public AMEDestructableObjectBase
{
	GENERATED_BODY()
public:
	AMEExplosiveObjectBase();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosiveObjectInfo", Meta = (AllowPrivateAccess = true))
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosiveObjectInfo", Meta = (AllowPrivateAccess = true))
		float ExplosionRadius;
};
