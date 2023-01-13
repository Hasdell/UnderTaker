// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "MEDestructableObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMEDestructableObjectBase : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMEDestructableObjectBase();

public:
	UFUNCTION(Server, Reliable)
		void ServerFadeAway();
protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartDeathTick();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopDeathTick();

	bool DeathTick(float DeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void BeginDestroy() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sound, Meta = (AllowPrivateAccess = true))
		class UFMODAudioComponent* FMODComponent;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "DestructibleComponent", Meta = (AllowPrivateAccess = true))
		class UDestructibleComponent* DestructibleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* DestructionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FadeAway", meta = (AllowPrivateAccess = true))
		float FadeAwayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FadeAway", meta = (AllowPrivateAccess = true))
		float FadeAwayWaitingTime;
	
	float CurFadeAwayTime{ 0 };

	FTimerHandle FadeAwayTimerHandle;
	FTimerHandle DeathTickTimerHandle;

	FDelegateHandle FadeAwayTickerHandle;
};
