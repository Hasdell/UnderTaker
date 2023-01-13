// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "MERewardBox.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API AMERewardBox : public AMEInteractionObjectBase
{
	GENERATED_BODY()
public:
	AMERewardBox();

	UFUNCTION(Server, Reliable)
	void ServerSetAnimInstance();
	void ServerSetAnimInstance_Implementation() { MultiCastSetAnimInstance(); }
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSetAnimInstance();

	void ServerInteraction_Implementation() override;
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastOpenBox();

	UFUNCTION(Server, Reliable)
		void ServerDisableCollision();
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastDisableCollision();
public:
	virtual void BeginPlay() override;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* BoxBody = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* BoxLid = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly , Category = "AnimInstance")
		class UMERewardBoxBodyAnimInstance* BoxBodyAnimInstance = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "AnimInstance")
		class UMERewardBoxLidAnimInstance* BoxLidAnimInstance = nullptr;
private:
	// Box_Open_Ani ���� ���� �� �ִ�
	// Box_Open_Dust_Eft ���� ���� �� ���� ����Ʈ
	// Box_Open_Sound ���� ���� �� ����
};
