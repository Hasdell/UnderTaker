// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectBase/MEInteractionObjectBase.h"
#include "Player/Base/MEPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Components/WidgetComponent.h"
#include "Multiplayer/MEGameModeBase.h"

#include "Net/UnrealNetwork.h"

#include "Kismet/KismetMathLibrary.h"

#include "Components/SlateWrapperTypes.h"

// Sets default values
AMEInteractionObjectBase::AMEInteractionObjectBase()
{
	bCanInteraction = true;

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);

	MarkWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DenoteWidget"));
	MarkWidgetComponent->SetupAttachment(RootComponent);

	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	MarkWidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> INTERACTION_BP(TEXT("/Game/UIs/WidgetBlueprints/ingame/mainplay/WB_ObjectInteraction"));
	static ConstructorHelpers::FClassFinder<UUserWidget> DENOTE_BP(TEXT("/Game/UIs/WidgetBlueprints/basecamp/basecamp/WB_object"));

	if (INTERACTION_BP.Succeeded())
	{
		InteractionWidgetComponent->SetWidgetClass(INTERACTION_BP.Class);
		InteractionWidgetComponent->SetDrawSize(FVector2D(162, 49.5));
	}
	if (DENOTE_BP.Succeeded())
	{
		MarkWidgetComponent->SetWidgetClass(DENOTE_BP.Class);
		MarkWidgetComponent->SetDrawSize(FVector2D(106, 106));
	}

	MarkWidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	InteractionWidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
}
void AMEInteractionObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_Client)
	{
		InteractionWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		MarkWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMEInteractionObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bCanInteraction)
	{
		if (InteractionWidgetComponent->GetVisibleFlag())
		{
			InteractionWidgetComponent->SetVisibility(false);
			MarkWidgetComponent->SetVisibility(false);
		}

		return;
	}
	else
	{
		if (!InteractionWidgetComponent->GetVisibleFlag())
		{
			InteractionWidgetComponent->SetVisibility(true);
			MarkWidgetComponent->SetVisibility(true);
		}
	}

	if (GetNetMode() == ENetMode::NM_Client)
	{
		if (!IsValid(LocalPlayer))
		{
			LocalPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
		}

		SetWidgetLookLocation();

		InteractionWidgetComponent->SetRelativeRotation(InteractionWidgetLookLocation.Rotation() - GetActorRotation());
		MarkWidgetComponent->SetRelativeRotation(MarkWidgetLookLocation.Rotation() - GetActorRotation());
	}

}
void AMEInteractionObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEInteractionObjectBase, bCanInteraction);
	DOREPLIFETIME(AMEInteractionObjectBase, InteractionTimer);
}
bool AMEInteractionObjectBase::IsPlayerOnInteractionDistance()
{
	UWorld* World = GetWorld();

	TArray<FOverlapResult> OverlapResults;
	TArray<AActor*> IgnoreActors;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);

	bool bResult = World->OverlapMultiByChannel( // 지정된 Collision FCollisionShape와 충돌한 액터 감지 
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel13,
		FCollisionShape::MakeSphere(InteractionDistance),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AActor* Target = OverlapResult.GetActor();

			if (Cast<AMEPlayerCharacter>(Target))
			{
				if (GetNetMode() == ENetMode::NM_Client)
				{
					AMEPlayerCharacter* PlayerCharacter = Cast<AMEPlayerCharacter>(Target);
					if (PlayerCharacter == Cast<AMEPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						ShowInteractionWidget();
					}
				}
				return true;
			}
		}
	}
	else
	{
		if (GetNetMode() == ENetMode::NM_Client)
		{
			HideInteractionWidget();
		}
	}

	return false;
}
bool AMEInteractionObjectBase::IsDisableInteraction()
{
	return !bCanInteraction;
}

void AMEInteractionObjectBase::SetWidgetLookLocation()
{
	if (IsValid(LocalPlayer))
	{
		if (Cast<AMEPlayerCharacter>(LocalPlayer))
		{
			AMEPlayerCharacter* PlayerCharacter = Cast<AMEPlayerCharacter>(LocalPlayer);

			FVector PlayerCameraLocation = PlayerCharacter->GetCameraComponent()->GetComponentLocation() + PlayerCharacter->GetCameraComponent()->GetRelativeLocation();

			InteractionWidgetLookLocation = PlayerCameraLocation - InteractionWidgetComponent->GetComponentLocation();
			MarkWidgetLookLocation = PlayerCameraLocation - MarkWidgetComponent->GetComponentLocation();
		}
	}
}

void AMEInteractionObjectBase::ShowInteractionWidget()
{
	MarkWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	InteractionWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
}
void AMEInteractionObjectBase::HideInteractionWidget()
{
	MarkWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	InteractionWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
}

