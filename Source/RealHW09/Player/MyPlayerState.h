#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class REALHW09_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	FString GetPlayerInfoString();

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxGuessCount;
	UPROPERTY(Replicated)
	FString PlayerNameString;
};
