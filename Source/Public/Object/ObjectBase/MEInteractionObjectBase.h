// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ObjectBase/MEObjectBase.h"
#include "MEInteractionObjectBase.generated.h"

UCLASS()
class MOONEATER_API AMEInteractionObjectBase : public AMEObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEInteractionObjectBase();

	UFUNCTION(Server, Reliable)
		void ServerInteraction();
	virtual void ServerInteraction_Implementation() {};
	UFUNCTION(Server, Reliable)
		void ServerInteractionOff();
	virtual void ServerInteractionOff_Implementation() {};

	bool IsPlayerOnInteractionDistance();
	bool IsDisableInteraction();
private:
	void SetWidgetLookLocation();

	void ShowInteractionWidget();
	void HideInteractionWidget();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
		class UWidgetComponent* InteractionWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
		class UWidgetComponent* MarkWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionInfo", meta = (AllowPrivateAccess = true))
		float InteractionCoolTime;

	UPROPERTY(Replicated)
		bool bCanInteraction;
	UPROPERTY(Replicated)
		float InteractionTimer;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionInfo", meta = (AllowPrivateAccess = true))
		float InteractionDistance;

	FVector InteractionWidgetLookLocation;
	FVector MarkWidgetLookLocation;

	class APawn* LocalPlayer;
};
