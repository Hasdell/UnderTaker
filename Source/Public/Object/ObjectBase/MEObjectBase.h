// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MEObjectBase.generated.h"

UENUM(BlueprintType)
enum class EObjectMoveDirection : uint8
{
	Up = 0,
	Down = 1
};
UENUM(BlueprintType)
enum class EIronBarOperationMode : uint8
{
	Normal = 0,
	Defense = 1
};
UCLASS()
class MOONEATER_API AMEObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEObjectBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Replicated)
		class UStaticMeshComponent* StaticMesh;
protected:
	class AMEGameModeBase* GameMode;
};
