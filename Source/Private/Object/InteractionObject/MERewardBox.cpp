// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/InteractionObject/MERewardBox.h"

#include "Components/SkeletalMeshComponent.h"

#include "Object/ObjectAnimInstance/MERewardBoxBodyAnimInstance.h"
#include "Object/ObjectAnimInstance/MERewardBoxLidAnimInstance.h"

#include "Components/BoxComponent.h"

#include "Components/WidgetComponent.h"

AMERewardBox::AMERewardBox()
{
	BoxBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BoxBody;

	BoxLid = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LidMesh"));
	BoxLid->SetupAttachment(RootComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	InteractionWidgetComponent->SetupAttachment(BoxCollision);
	MarkWidgetComponent->SetupAttachment(BoxCollision);
}

void AMERewardBox::MultiCastSetAnimInstance_Implementation()
{
	BoxBodyAnimInstance = Cast<UMERewardBoxBodyAnimInstance>(BoxBody->GetAnimInstance());
	BoxLidAnimInstance = Cast<UMERewardBoxLidAnimInstance>(BoxLid->GetAnimInstance());
}

void AMERewardBox::ServerInteraction_Implementation()
{
	if (!bCanInteraction)
		return;

	ServerSetAnimInstance();

	BoxLidAnimInstance->BoxOpenDelegate.BindUFunction(this, FName("ServerDisableCollision"));

	MultiCastOpenBox();
}
void AMERewardBox::ServerDisableCollision_Implementation()
{
	MultiCastDisableCollision();
}
void AMERewardBox::MultiCastDisableCollision_Implementation()
{
	BoxLid->SetCollisionProfileName(FName(TEXT("BlockAll")));
	BoxBody->SetCollisionProfileName(FName(TEXT("BlockAll")));

	BoxCollision->SetCollisionProfileName(FName(TEXT("NoCollision")));
}
void AMERewardBox::BeginPlay()
{
	Super::BeginPlay();
}
void AMERewardBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
void AMERewardBox::MultiCastOpenBox_Implementation()
{
	BoxBodyAnimInstance->PlayOpenAnimation();
	BoxLidAnimInstance->PlayOpenAnimation();

	BoxLid->SetCollisionProfileName(FName(TEXT("BlockAll")));
	BoxBody->SetCollisionProfileName(FName(TEXT("BlockAll")));

	bCanInteraction = false;
	UE_LOG(LogTemp, Log, TEXT("Open_Box"));
}