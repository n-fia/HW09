#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AMyGameStateBase;
class AMyPlayerState;
class AMyPlayerController;

UCLASS()
class REALHW09_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;

	void PrintChatMessageString(AMyPlayerController* InChattingPlayerController, const FString& InChatMessageString);

protected:
	FString CreateSecretNumber();
	FString JudgeNumber(const FString& SecretNumberString, const FString& CorrectNumberString);

	void IncreaseGuessCount(AMyPlayerController* InChattingPlayerController);

	void ResetGame();
	void IsWinGame(AMyPlayerController* InChattingPlayerController, int StrikeCount);

	void NotifyText(FString& NotifyText);

	bool bIsNumberString(const FString& InNumberString);
	bool bIsChanced(AMyPlayerController* InChattingPlayerController);

	void ChangeTurn();
	void TimeStart(AMyPlayerController* MyPC);
	void UpdateTime();
	FTimerHandle HUDTimer;

	UPROPERTY()
	TObjectPtr<AMyPlayerController> TurnPlayerController;
	
	FString SecretNumber;
	TArray<TObjectPtr<AMyPlayerController>> AllPlayerControllers;
};
