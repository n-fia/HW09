#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMyTimeWidget;
class UMyUserWidget;
class UUserWidget;

UCLASS()
class REALHW09_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	virtual void BeginPlay() override;
	void PrintChatMessageString(const FString& InChatMessageString) const;
	
	UFUNCTION()
	void UpdateHUD(int32 CurrentTime);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatString(const FString& InChatString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatString(const FString& InChatString);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;	
	UPROPERTY(Replicated,BlueprintReadOnly)
	FString NotificationText;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyUserWidget> ChatInputWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMyUserWidget> ChatInputWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyTimeWidget> TimerWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMyTimeWidget> TimerWidgetInstance;
	

};
