#include "Game/MyGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/MyPlayerController.h"

void AMyGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority()==false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
		if (IsValid(PC)==true)
		{
			AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC);
			if (IsValid(MyPC)==true)
			{
				FString NotifyString = InNameString+TEXT(" joined");
				MyPC->PrintChatMessageString(NotifyString);
			}
		}
	}
}

AMyGameStateBase::AMyGameStateBase()
	: CurrentTime(15)
	, TurnTime(15)
{
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, CurrentTime);
}

void AMyGameStateBase::OnRep_UpdateTime()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
		{
			MyPC->UpdateHUD(CurrentTime);
		}
	}
}

