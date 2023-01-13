// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Base/METriggerComponent.h"
#include "MEElevatorTriggerComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOONEATER_API UMEElevatorTriggerComponent : public UMETriggerComponent
{
	GENERATED_BODY()
		// Fill out your copyright notice in the Description page of Project Settings.

public:
	UMEElevatorTriggerComponent();

	int GetInPlayerNum();
	int GetAllPlayerNum();
};
