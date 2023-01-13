// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEPlayerUIManager.h"

#include "Player/Base/MEPlayerCharacter.h"

#include "Weapon/Base/MEPlayerWeapon.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

#include "Kismet/KismetMathLibrary.h"

#include "Engine/UserInterfaceSettings.h"

#include "Net/UnrealNetwork.h"
#include "UI/MEYugusUI.h"

// Sets default values
AMEPlayerUIManager::AMEPlayerUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	CrosshairWidget = nullptr;
	InGameWidget = nullptr;

	OldDPIScaling = 0.f;


	static ConstructorHelpers::FClassFinder<UUserWidget> GAMEOVER_WIDGETBP(TEXT("/Game/UIs/WidgetBlueprints/ingame/WB_GameOver.WB_GameOver_C"));

	if (GAMEOVER_WIDGETBP.Succeeded())
	{
		GameOverWidgetBlueprint = GAMEOVER_WIDGETBP.Class;
	}
	else
	{
		ME_LOG_ERROR_FAILED(TEXT("Load game over widget blueprint."));
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> GAMECLEAR_WIDGETBP(TEXT("/Game/UIs/WidgetBlueprints/ingame/WB_GameClear.WB_GameClear_C"));

	if (GAMECLEAR_WIDGETBP.Succeeded())
	{
		GameClearWidgetBlueprint = GAMECLEAR_WIDGETBP.Class;
	}
	else
	{
		ME_LOG_ERROR_FAILED(TEXT("Load game clear widget blueprint."));
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> SPECTATOR_WIDGETBP(TEXT("/Game/UIs/WidgetBlueprints/ingame/WB_spectating.WB_spectating_C"));

	if (SPECTATOR_WIDGETBP.Succeeded())
	{
		SpectatorWidgetBlueprint = SPECTATOR_WIDGETBP.Class;
	}
	else
	{
		ME_LOG_ERROR_FAILED(TEXT("Load game spectator widget blueprint."));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> KnockDown_WIDGETBP(TEXT("/Game/UIs/WidgetBlueprints/ingame/mainplay/WB_KnockDown"));

	if (KnockDown_WIDGETBP.Succeeded())
	{
		KnockDownWidgetBlueprint = KnockDown_WIDGETBP.Class;
	}
	else
	{
		ME_LOG_ERROR_FAILED(TEXT("Load game KnockDown widget blueprint."));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BridgePopup_WIDGETBP(TEXT("/Game/UIs/WidgetBlueprints/ingame/WB_bridgepopup"));

	if (BridgePopup_WIDGETBP.Succeeded())
	{
		BridgePopupWidgetBlueprint = BridgePopup_WIDGETBP.Class;
	}
	else
	{
		ME_LOG_ERROR_FAILED(TEXT("Load game BridgePopupwidget blueprint."));
	}

	ItemNum = 0;
}

// Called when the game starts or when spawned
void AMEPlayerUIManager::BeginPlay()
{
	Super::BeginPlay();

	UIDelay = 1.f;
	CrosshairHitDelay = 0.5f;
	GetAttackDelay = 1.f;
	InGameUIOnOff = true;

	SpectatorWidget = CreateWidget(GetWorld(), SpectatorWidgetBlueprint);

	GameState = GetWorld()->GetGameState<AMEGameStateBase>();
}

void AMEPlayerUIManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMEPlayerUIManager, CrosshairWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, InGameWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, GameOverWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, GameClearWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, SpectatorWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, KnockDownWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, BridgePopupWidgetBlueprint);
	DOREPLIFETIME(AMEPlayerUIManager, PlayerCharacter);
}

// Called every frame
void AMEPlayerUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UIDelay > 0)
	{
		UIDelay -= DeltaTime;
		return;
	}

	if (PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter->GetController() != nullptr && GetNetMode() == ENetMode::NM_Client)
	{
		if (CrosshairHitDelayCurrent <= 0.f && bIsCrosshairHitDelay)
		{
			ClientHideCrosshairHit();

			bIsCrosshairHitDelay = false;
		}
		else
		{
			CrosshairHitDelayCurrent -= DeltaTime;
		}
		if (CrosshairHeadShotDelayCurrent <= 0.f && bIsCrosshairHeadShotDelay)
		{
			ClientHideCrosshairHeadShot();
			
			bIsCrosshairHeadShotDelay = false;
		}
		else
		{
			CrosshairHeadShotDelayCurrent -= DeltaTime;
		}
		if (CrosshairShootDelayCurrent <= 0.f && bIsCrosshairShootDelay)
		{
			ClientHideCrosshairShoot();

			bIsCrosshairShootDelay = false;
		}
		else
		{
			CrosshairShootDelayCurrent -= DeltaTime;
		}
		if (GetAttackDelayCurrent <= 0.f)
		{
			ClientHideGetAttack();
		}
		else
		{
			GetAttackDelayCurrent -= DeltaTime;
		}

		if (InGameUIOnOff == true)
		{
			if (InGameWidgetBlueprint != nullptr)
			{
				if (InGameWidget == nullptr)
				{
					InGameWidget = CreateWidget(GetWorld(), InGameWidgetBlueprint);
				}
				else
				{
					if (!(InGameWidget->IsInViewport()))
					{
						ClientShowInGame();
					}
				}
			}

			if (GameOverWidgetBlueprint != nullptr)
			{
				if (GameOverWidget == nullptr)
				{
					GameOverWidget = CreateWidget(GetWorld(), GameOverWidgetBlueprint);
				}
			}
			
			if (GameClearWidgetBlueprint != nullptr)
			{
				if (GameClearWidget == nullptr)
				{
					GameClearWidget = CreateWidget(GetWorld(), GameClearWidgetBlueprint);
				}
			}

			if (KnockDownWidgetBlueprint != nullptr)
			{
				if (KnockDownWidget == nullptr)
				{
					KnockDownWidget = CreateWidget(GetWorld(), KnockDownWidgetBlueprint);
				}
			}

			if (BridgePopupWidgetBlueprint != nullptr)
			{
				if (BridgePopupWidget == nullptr)
				{
					BridgePopupWidget = CreateWidget(GetWorld(), BridgePopupWidgetBlueprint);
				}
			}

			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			const float DPIScaling = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

			if (DPIScaling == OldDPIScaling && CrosshairWidget->IsInViewport())
			{
				return;
			}

			OldDPIScaling = DPIScaling;

			if (CrosshairWidgetBlueprint != nullptr)
			{
				if (CrosshairWidget == nullptr)
				{
					CrosshairWidget = CreateWidget(GetWorld(), CrosshairWidgetBlueprint);
				}
				else
				{
					ClientShowCrosshair();
				}
			}
		}
		else
		{
			if (InGameWidget != nullptr)
			{
				if (InGameWidget->IsInViewport())
				{
					ClientHideInGame();
				}
			}
			if (CrosshairWidget != nullptr)
			{
				if (CrosshairWidget->IsInViewport())
				{
					ClientHideCrosshair();
				}
			}
		}

		// if(GameState)
		// {
		// 	if(GameState->IsBoss1Clear())
		// 	{				
		// 		if (!GameClearWidget->IsInViewport())
		// 		{
		// 			ClientShowGameClear();
		// 		}
		// 	}
		// }
	}
}

void AMEPlayerUIManager::ClientShowDamageUI_Implementation(float Damage)
{
	if(DamageUIBlueprint)
	{
		UUserWidget* NewDamageUI = CreateWidget<UUserWidget>(GetWorld(), DamageUIBlueprint);
	}
}

void AMEPlayerUIManager::ClientShowCrosshair_Implementation()
{
	ME_INVALID_RETURN(CrosshairWidget);

	if (PlayerCharacter == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = PlayerCharacter->GetController<APlayerController>();
	UCameraComponent* CameraComponent = PlayerCharacter->GetCameraComponent();
	AMEPlayerWeapon* PlayerWeapon = PlayerCharacter->GetWeapon();


	if (PlayerController == nullptr)
	{
		return;
	}

	if (PlayerWeapon == nullptr)
	{
		return;
	}

	TArray<UWidget*> CrosshairArray;
	
	CrosshairArray.Add(CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal")));
	CrosshairArray.Add(CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Shoot")));
	CrosshairArray.Add(CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HIT")));
	CrosshairArray.Add(CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HEADSHOT")));
	CrosshairArray.Add(CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_reload")));

	TArray<UCanvasPanelSlot*> PanelArray;

	for (auto Crosshair : CrosshairArray)
	{
		if (Crosshair == nullptr)
			return;

		PanelArray.Add(UWidgetLayoutLibrary::SlotAsCanvasSlot(Crosshair));
	}

	if (PanelArray.Num() != CrosshairArray.Num())
		return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	const float DPIScaling = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

	float ViewportRatioX = ViewportSize.X / ViewportSize.Y;
	float ViewportRatioY = ViewportSize.Y / ViewportSize.X;

	float Distance = PlayerWeapon->GetMaxDistance();
	float Radius = PlayerWeapon->GetRadius();

	FVector Center = CameraComponent->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(PlayerController->GetControlRotation()) * Distance);
	FVector Corner = Center + (UKismetMathLibrary::GetRightVector(PlayerController->GetControlRotation()) * Radius);

	FVector2D CenterScreen;
	PlayerController->ProjectWorldLocationToScreen(Center, CenterScreen);

	FVector2D CornerScreen;
	PlayerController->ProjectWorldLocationToScreen(Corner, CornerScreen);

	float CrosshairRadius = (CenterScreen - CornerScreen).Size();

	CrosshairRadiusWorld = (Center - Corner).Size();
	CrosshairRadiusScreen = CrosshairRadius;

	for (auto Panel : PanelArray)
	{
		Panel->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		Panel->SetAlignment(FVector2D(0.5f, 0.5f));

		Panel->SetSize(FVector2D(CrosshairRadius * 2 / DPIScaling));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%.2lf"), CrosshairRadius);
	

	if (CrosshairWidget->IsInViewport())
	{
		return;
	}

	CrosshairWidget->AddToViewport();

	CrosshairArray[0]->SetVisibility(ESlateVisibility::Visible);

	ServerSetCrosshairRadiusWorld(CrosshairRadiusWorld);
	ServerSetCrosshairRadiusScreen(CrosshairRadiusScreen);
}

void AMEPlayerUIManager::ClientShowInGame_Implementation()
{
	if (!InGameWidget->IsInViewport())
	{
		InGameWidget->AddToViewport();
	}
}

void AMEPlayerUIManager::ClientHideCrosshair_Implementation()
{
	if (CrosshairWidget->IsInViewport())
	{
		CrosshairWidget->RemoveFromViewport();
	}
}

void AMEPlayerUIManager::ClientHideInGame_Implementation()
{
	if (InGameWidget->IsInViewport())
	{
		InGameWidget->RemoveFromViewport();
	}
}

void AMEPlayerUIManager::ServerSetCrosshairRadiusWorld_Implementation(float CrosshairRadiusWorldArg)
{
	CrosshairRadiusWorld = CrosshairRadiusWorldArg;
}

void AMEPlayerUIManager::ServerSetCrosshairRadiusScreen_Implementation(float CrosshairRadiusScreenArg)
{
	CrosshairRadiusScreen = CrosshairRadiusScreenArg;
}

void AMEPlayerUIManager::MulticastSetCrosshairRadiusWorld_Implementation(float CrosshairRadiusWorldArg)
{
	CrosshairRadiusWorld = CrosshairRadiusWorldArg;
}

void AMEPlayerUIManager::MulticastSetCrosshairRadiusScreen_Implementation(float CrosshairRadiusScreenArg)
{
	CrosshairRadiusScreen = CrosshairRadiusScreenArg;
}

void AMEPlayerUIManager::ShowUICrosshair()
{
	// CrosshairWidget->AddToViewport();
}
void AMEPlayerUIManager::ShowUIInGame()
{
	InGameWidget->AddToViewport();
}
void AMEPlayerUIManager::HideUICrosshair()
{
	CrosshairWidget->RemoveFromViewport();
}
void AMEPlayerUIManager::HideUIInGame()
{
	InGameWidget->RemoveFromViewport();
}

void AMEPlayerUIManager::ClientShowGameOver_Implementation()
{
	if (!IsValid(GameOverWidget))
	{
		return;
	}
	if (!(GameOverWidget->IsInViewport()))
	{
		if (bIsShowGameOver)
			return;

		if (KnockDownWidget->IsInViewport())
			KnockDownWidget->RemoveFromParent();

		InGameUIOnOff = false;
		GameOverWidget->AddToViewport();
		bIsShowGameOver = true;
	}
}

void AMEPlayerUIManager::ClientShowGameClear_Implementation()
{
	if (!IsValid(GameClearWidget))
	{
		return;
	}
	if (!(GameClearWidget->IsInViewport()))
	{
		if (bIsShowGameClear)
			return;

		if (KnockDownWidget->IsInViewport())
			KnockDownWidget->RemoveFromParent();

		InGameUIOnOff = false;
		GameClearWidget->AddToViewport();
		bIsShowGameClear = true;
	}
}

void AMEPlayerUIManager::ClientShowKnockedOut_Implementation()
{
	if (!IsValid(KnockDownWidget))
	{
		return;
	}
	if (!(KnockDownWidget->IsInViewport()))
	{
		if (bIsShowGameClear)
			return;
		if (bIsShowGameOver)
			return;

		KnockDownWidget->AddToViewport();

		InGameUIOnOff = false;
	}
}

void AMEPlayerUIManager::ClientHideKnockedOut_Implementation()
{
	if (!IsValid(KnockDownWidget))
	{
		return;
	}

	if (KnockDownWidget->IsInViewport())
	{
		KnockDownWidget->RemoveFromParent();

		InGameUIOnOff = true;
	}
}

void AMEPlayerUIManager::ClientShowRevive_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* ReviveWidget = InGameWidget->GetWidgetFromName(TEXT("WB_ReviveInteraction"));

	ME_INVALID_RETURN(ReviveWidget);

	ReviveWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMEPlayerUIManager::ClientHideRevive_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* ReviveWidget = InGameWidget->GetWidgetFromName(TEXT("WB_ReviveInteraction"));

	ME_INVALID_RETURN(ReviveWidget);

	ReviveWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void AMEPlayerUIManager::ClientShowElevatorUI_Implementation()
{
	/*
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* InteractionUI = InGameWidget->GetWidgetFromName(TEXT("WB_ObjectInteraction"));

	ME_INVALID_RETURN(InteractionUI);

	InteractionUI->SetVisibility(ESlateVisibility::Visible);
	*/
}
void AMEPlayerUIManager::ClientHideElevatorUI_Implementation()
{
	/*
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* InteractionUI = InGameWidget->GetWidgetFromName(TEXT("WB_ObjectInteraction"));

	ME_INVALID_RETURN(InteractionUI);

	InteractionUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	*/
}
void AMEPlayerUIManager::ClientShowItemGet_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* ItemInteractionWidget = InGameWidget->GetWidgetFromName(TEXT("WB_ItemInteraction"));
	UWidget* ItemPopupWidget = InGameWidget->GetWidgetFromName(TEXT("WB_itempopup"));

	ME_INVALID_RETURN(ItemInteractionWidget);
	ME_INVALID_RETURN(ItemPopupWidget);

	ItemInteractionWidget->SetVisibility(ESlateVisibility::Visible);
	ItemPopupWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMEPlayerUIManager::ClientHideItemGet_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* ItemInteractionWidget = InGameWidget->GetWidgetFromName(TEXT("WB_ItemInteraction"));
	UWidget* ItemPopupWidget = InGameWidget->GetWidgetFromName(TEXT("WB_itempopup"));

	ME_INVALID_RETURN(ItemInteractionWidget);
	ME_INVALID_RETURN(ItemPopupWidget);

	ItemInteractionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemPopupWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AMEPlayerUIManager::ClientShowCrosshairShoot_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairShoot = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Shoot"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairShoot);

	bIsCrosshairShootDelay = true;

	CrosshairNormal->SetVisibility(ESlateVisibility::Hidden);
	CrosshairShoot->SetVisibility(ESlateVisibility::Visible);

	CrosshairShootDelayCurrent = CrosshairHitDelay;
}

void AMEPlayerUIManager::ClientHideCrosshairShoot_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairShoot = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Shoot"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairShoot);

	CrosshairNormal->SetVisibility(ESlateVisibility::Visible);
	CrosshairShoot->SetVisibility(ESlateVisibility::Hidden);
}

void AMEPlayerUIManager::ClientShowCrosshairHit_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairHit = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HIT"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairHit);

	bIsCrosshairHitDelay = true;

	CrosshairNormal->SetVisibility(ESlateVisibility::Hidden);
	CrosshairHit->SetVisibility(ESlateVisibility::Visible);

	CrosshairHitDelayCurrent = CrosshairHitDelay;
}

void AMEPlayerUIManager::ClientHideCrosshairHit_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairHit = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HIT"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairHit);

	CrosshairNormal->SetVisibility(ESlateVisibility::Visible);
	CrosshairHit->SetVisibility(ESlateVisibility::Hidden);
}

void AMEPlayerUIManager::ClientShowCrosshairHeadShot_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairHeadShot = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HEADSHOT"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairHeadShot);

	bIsCrosshairHeadShotDelay = true;

	CrosshairNormal->SetVisibility(ESlateVisibility::Hidden);
	CrosshairHeadShot->SetVisibility(ESlateVisibility::Visible);

	CrosshairHeadShotDelayCurrent = CrosshairHitDelay;
}

void AMEPlayerUIManager::ClientHideCrosshairHeadShot_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairHeadShot = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_HEADSHOT"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairHeadShot);

	CrosshairNormal->SetVisibility(ESlateVisibility::Visible);
	CrosshairHeadShot->SetVisibility(ESlateVisibility::Hidden);
}

void AMEPlayerUIManager::ClientShowCrosshairReload_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairReload = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_reload"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairReload);

	CrosshairNormal->SetVisibility(ESlateVisibility::Hidden);
	CrosshairReload->SetVisibility(ESlateVisibility::Visible);
}

void AMEPlayerUIManager::ClientHideCrosshairReload_Implementation()
{
	if (!IsValid(CrosshairWidget))
	{
		return;
	}

	UWidget* CrosshairNormal = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_Normal"));
	UWidget* CrosshairReload = CrosshairWidget->GetWidgetFromName(TEXT("Image_Crosshair_reload"));

	ME_INVALID_RETURN(CrosshairNormal);
	ME_INVALID_RETURN(CrosshairReload);

	CrosshairReload->SetVisibility(ESlateVisibility::Hidden);
	CrosshairNormal->SetVisibility(ESlateVisibility::Visible);
}

void AMEPlayerUIManager::ClientShowGetAttack_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* GetAttackWidget = InGameWidget->GetWidgetFromName(TEXT("WB_attacked"));

	ME_INVALID_RETURN(GetAttackWidget);

	GetAttackWidget->SetVisibility(ESlateVisibility::Visible);

	GetAttackDelayCurrent = GetAttackDelay;
}

void AMEPlayerUIManager::ClientHideGetAttack_Implementation()
{
	if (!IsValid(InGameWidget))
	{
		return;
	}

	UWidget* GetAttackWidget = InGameWidget->GetWidgetFromName(TEXT("WB_attacked"));

	ME_INVALID_RETURN(GetAttackWidget);

	GetAttackWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMEPlayerUIManager::ClientAddWidget_Implementation(TSubclassOf<UUserWidget> WidgetBlueprint)
{
	if(WidgetBlueprint)
	{
		UUserWidget* NewWidget = CreateWidget(GetWorld(), WidgetBlueprint);
		NewWidget->AddToViewport();
	}
}

void AMEPlayerUIManager::ClientAddBossUI_Implementation(TSubclassOf<UMEYugusUI> BossUIBlueprint)
{
	if(BossUIBlueprint)
	{
		UMEYugusUI* NewBossUI = CreateWidget<UMEYugusUI>(GetWorld(), BossUIBlueprint);
		NewBossUI->AddToViewport();
	}
}

void AMEPlayerUIManager::ClientShowSpectator_Implementation()
{
	if (!IsValid(SpectatorWidget))
	{
		return;
	}
	if (!SpectatorWidget->IsInViewport())
	{
		SpectatorWidget->AddToViewport();
	}
}

void AMEPlayerUIManager::ClientHideSpectator_Implementation()
{
	if (IsValid(SpectatorWidget))
	{
		return;
	}
	if (SpectatorWidget->IsInViewport())
	{
		SpectatorWidget->RemoveFromViewport();
	}
}

void AMEPlayerUIManager::UpdateSpectatorWidget(FString Nickname)
{
	UTextBlock* NameWidget = Cast<UTextBlock>(SpectatorWidget->GetWidgetFromName(TEXT("spetcing_name")));

	if (IsValid(NameWidget))
	{
		NameWidget->SetText(FText::FromString(Nickname));
	}
}
void AMEPlayerUIManager::ClientUpdateInGameWidget_Implementation()
{
	if (IsValid(InGameWidget))
	{
		InGameWidget = nullptr;
		InGameWidget = CreateWidget(GetWorld(), InGameWidgetBlueprint);
	}
}

void AMEPlayerUIManager::ClientSetItemNum_Implementation(int value)
{
	ItemNum = value;
	ClientShowItemGet();
}

