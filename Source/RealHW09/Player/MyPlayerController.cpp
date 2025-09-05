#include "Player/MyPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "UI/MyUserWidget.h"
#include "UI/MyTimeWidget.h"
#include "EngineUtils.h"
#include "MyPlayerState.h"
#include "RealHW09.h"
#include "Game/MyGameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		if (IsValid(ChatInputWidgetClass) == true)
		{
			ChatInputWidgetInstance = CreateWidget<UMyUserWidget>(this, ChatInputWidgetClass);
			if (IsValid(ChatInputWidgetInstance) == true)
			{
				ChatInputWidgetInstance->AddToViewport();
			}
		}
		if (IsValid(NotificationTextWidgetClass) == true)
		{
			NotificationTextWidgetInstance  = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
			if (IsValid(NotificationTextWidgetInstance ) == true)
			{
				NotificationTextWidgetInstance ->AddToViewport();
			}
		}
		if (IsValid(TimerWidgetClass) == true)
		{
			TimerWidgetInstance  = CreateWidget<UMyTimeWidget>(this, TimerWidgetClass);
			if (IsValid(TimerWidgetInstance ) == true)
			{
				TimerWidgetInstance ->AddToViewport();
			}
		}
	}
}

void AMyPlayerController::PrintChatMessageString(const FString& InChatMessageString) const
{
	MyFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AMyPlayerController::UpdateHUD(int32 CurrentTime)
{
	if (TimerWidgetInstance)
	{
		UMyTimeWidget* TimerWidget = Cast<UMyTimeWidget>(TimerWidgetInstance);
		if (TimerWidget)
		{
			TimerWidget->SetTimeText(FString::Printf(TEXT("Time: %d"),CurrentTime));
		}
	}
}

void AMyPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerController,NotificationText);
}

void AMyPlayerController::ClientRPCPrintChatString_Implementation(const FString& InChatString)
{
	PrintChatMessageString(InChatString);
}

void AMyPlayerController::ServerRPCPrintChatString_Implementation(const FString& InChatString)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		AMyGameModeBase* MyGM = Cast<AMyGameModeBase>(GameMode);
		if (IsValid(MyGM) == true)
		{
			MyGM->PrintChatMessageString(this, InChatString);
		}
	}
}