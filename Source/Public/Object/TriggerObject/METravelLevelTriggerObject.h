// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "METravelLevelTriggerObject.generated.h"

/**
 * 
 */

UCLASS()
class MOONEATER_API AMETravelLevelTriggerObject : public AMEObjectBase
{
	GENERATED_BODY()
public:
	AMETravelLevelTriggerObject();

private:
	UFUNCTION(Server, Reliable)
		void ServerPlayerListAdd(class AMEPlayerCharacter* Player);
	UFUNCTION(Server, Reliable)
		void ServerPlayerListRemove(class AMEPlayerCharacter* Player);

	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayerListAdd(class AMEPlayerCharacter* Player);
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastPlayerListRemove(class AMEPlayerCharacter* Player);

	UFUNCTION()
		void OnTriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnTriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (AllowPrivateAccess = true))
		class UBoxComponent* TriggerBox;

	UPROPERTY(Replicated)
		TArray<class AMEPlayerCharacter*> PlayerList;

	UPROPERTY(Replicated)
		bool bIsCalledTravelLevel;
};
