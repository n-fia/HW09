#include "Player/MyPlayerState.h"

#include "MyPlayerController.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
	: CurrentGuessCount(0)
	, MaxGuessCount(3)
	, PlayerNameString(TEXT("NONE"))
{
	bReplicates = true;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
}

FString AMyPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
