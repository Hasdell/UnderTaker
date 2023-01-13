// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEIronBarTriggerObject.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	AllPlayer = 0,
	BasscardDead = 1
};

UCLASS()
class MOONEATER_API AMEIronBarTriggerObject : public AMEObjectBase
{
	GENERATED_BODY()
public:
	AMEIronBarTriggerObject();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (AllowPrivateAccess = true))
		ETriggerType TriggerType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "IronBar", meta = (AllowPrivateAccess = true))
		class AMEIronBar* IronBar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Monster", meta = (AllowPrivateAccess = true))
		class AMEBasscardCharacter* Basscard;

	UPROPERTY(Replicated)
		TArray<class AMEPlayerCharacter*> PlayerList;

	UPROPERTY(Replicated)
		bool bIsCalledIronBarOperation;
};
