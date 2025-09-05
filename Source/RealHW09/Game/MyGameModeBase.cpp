#include "Game/MyGameModeBase.h"

#include "EngineUtils.h"
#include "MyGameStateBase.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"

AMyGameModeBase::AMyGameModeBase()
	:TurnPlayerController(nullptr)
{
}

void AMyGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(NewPlayer);
	if (IsValid(MyPlayerController) == true)
	{		
		AllPlayerControllers.Add(MyPlayerController);

		AMyPlayerState* MYPS = MyPlayerController->GetPlayerState<AMyPlayerState>();
		if (IsValid(MYPS) == true)
		{
			MYPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AMyGameStateBase* CXGameStateBase = GetGameState<AMyGameStateBase>();
		if (IsValid(CXGameStateBase) == true)
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(MYPS->PlayerNameString);
		}

		TimeStart(MyPlayerController);
	}
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SecretNumber = CreateSecretNumber();
}

FString AMyGameModeBase::CreateSecretNumber()
{
	FString SecretNumbers;
	for (int32 i = 0; i < 3; i++)
	{
		int32 Temp = FMath::RandRange(1, 9);
		SecretNumbers.Append(FString::FromInt(Temp));
	}
	return SecretNumbers;
}

bool AMyGameModeBase::bIsNumberString(const FString& InNumberString)
{
	if (InNumberString.Len()!=3) return false;
	
	for (TCHAR C : InNumberString)
	{
		if (FChar::IsDigit(C)==false || C=='0')
		{
			return false;
		}
	}
	return true;
}

FString AMyGameModeBase::JudgeNumber(const FString& SecretNumberString, const FString& CorrectNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (SecretNumberString[i] == CorrectNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), CorrectNumberString[i]);
			if (SecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AMyGameModeBase::IncreaseGuessCount(AMyPlayerController* InChattingPlayerController)
{
	AMyPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AMyPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void AMyGameModeBase::PrintChatMessageString(AMyPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	AMyPlayerState* PS = InChattingPlayerController->GetPlayerState<AMyPlayerState>();
	if (TurnPlayerController==InChattingPlayerController && bIsNumberString(InChatMessageString))
	{
		if (bIsChanced(InChattingPlayerController))
		{
			IncreaseGuessCount(InChattingPlayerController);
			FString JudgeResultString = JudgeNumber(SecretNumber, InChatMessageString);
			for (AMyPlayerController* MyPC : AllPlayerControllers)
			{
				AMyPlayerController* MyPlayerController = MyPC;
				if (IsValid(MyPlayerController))
				{
					if (IsValid(PS))
					{
						FString CombinedMessageString = PS->GetPlayerInfoString() + InChatMessageString + TEXT(" -> ") + JudgeResultString;
						MyPlayerController->ClientRPCPrintChatString(CombinedMessageString);
	            
						int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
						IsWinGame(InChattingPlayerController, StrikeCount);
					}
				}
			}
		}
	}
	else
	{
		for (AMyPlayerController* MyPC : AllPlayerControllers)
		{
			AMyPlayerController* MyPlayerController = MyPC;
			if (IsValid(MyPlayerController) && PS)
			{
				MyPlayerController->ClientRPCPrintChatString(PS->GetPlayerInfoString()+InChatMessageString);
			}
		}
	}
}

void AMyGameModeBase::ResetGame()
{
	SecretNumber = CreateSecretNumber();

	for (const auto& MyPlayerController : AllPlayerControllers)
	{
		AMyPlayerState* MyPlayerState = MyPlayerController->GetPlayerState<AMyPlayerState>();
		if (IsValid(MyPlayerState) == true)
		{
			MyPlayerState->CurrentGuessCount = 0;
		}
	}
}

void AMyGameModeBase::IsWinGame(AMyPlayerController* InChattingPlayerController, int StrikeCount)
{
	if (3==StrikeCount)
	{
		AMyPlayerState* MyPlayerState = InChattingPlayerController->GetPlayerState<AMyPlayerState>();
		for (const auto& MyPlayerController : AllPlayerControllers)
		{
			if (IsValid(MyPlayerState) == true)
			{
				FString CombinedMessageString = MyPlayerState->PlayerNameString + TEXT(" Win!\nNew Game Is Start!");
				NotifyText(CombinedMessageString);
				ResetGame();
			}
		}
	}
	else
	{
		for (const auto& PlayerController : AllPlayerControllers)
		{
			AMyPlayerState* PS = PlayerController->GetPlayerState<AMyPlayerState>();
			if (IsValid(PS) == true)
			{
				if (PS->CurrentGuessCount != PS->MaxGuessCount)
				{
					return;
				}
			}
		}
		for (const auto& MyPlayerController : AllPlayerControllers)
		{
			FString NotifyString= TEXT("Draw!\nNew Game Is Start");
			NotifyText(NotifyString);
			ResetGame();
		}
	}
	ChangeTurn();
}

void AMyGameModeBase::NotifyText(FString& NotifyText)
{
	for (const auto& MyPlayerController : AllPlayerControllers)
	{
		MyPlayerController->NotificationText = NotifyText;
	}
}

bool AMyGameModeBase::bIsChanced(AMyPlayerController* InChattingPlayerController)
{
	AMyPlayerState* MyPlayerState = InChattingPlayerController->GetPlayerState<AMyPlayerState>();
	if (IsValid(MyPlayerState))
	{
		if (MyPlayerState->CurrentGuessCount >= MyPlayerState->MaxGuessCount)
		{
			InChattingPlayerController->NotificationText = TEXT("You Don't Have A Chance");
			return false;
		}
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("[GameModeBase] bIsChanced"))
	return false;
}

void AMyGameModeBase::ChangeTurn()
{
	FString CombinedMessageString;
	
	for (const auto& PC : AllPlayerControllers)
	{
		if (PC==TurnPlayerController)
		{
			if (AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>())
			{
				if (PS->CurrentGuessCount >= PS->MaxGuessCount)
				{
					TurnPlayerController=nullptr;
					continue;
				}
			}
		}
		if (AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>())
		{
			if (PS->CurrentGuessCount < PS->MaxGuessCount)
			{
				TurnPlayerController=PC;
				break;
			}
		}
	}
	if (!TurnPlayerController)
	{
		FString Text = "Draw!\nRestartGame";
		NotifyText(Text);
		ResetGame();
	}
	if (AMyPlayerState* PS = TurnPlayerController->GetPlayerState<AMyPlayerState>())
	{
		AMyGameStateBase* GS = GetGameState<AMyGameStateBase>();
		GS->CurrentTime=GS->TurnTime;
		PS->CurrentGuessCount++;
		FString NotifyString = PS->PlayerNameString + TEXT(" Turn!");
		NotifyText(NotifyString);
	}
}

void AMyGameModeBase::TimeStart(AMyPlayerController* MyPC)
{
	if (!TurnPlayerController)
	{
		TurnPlayerController=MyPC;
		if (AMyPlayerState* PS = TurnPlayerController->GetPlayerState<AMyPlayerState>())
		{
			FString NotifyString = PS->PlayerNameString + TEXT(" Turn!");
			NotifyText(NotifyString);
		}
		GetWorld()->GetTimerManager().SetTimer(HUDTimer, this, &AMyGameModeBase::UpdateTime,1.f,true);
	}
}

void AMyGameModeBase::UpdateTime()
{
	AMyGameStateBase* GS = GetGameState<AMyGameStateBase>();
	if (GS)
	{
		if (GS->CurrentTime != 0)
		{
			GS->CurrentTime--;
		}
		else ChangeTurn();
	}
}

