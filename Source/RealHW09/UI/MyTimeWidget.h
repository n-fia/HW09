#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTimeWidget.generated.h"

class UTextBlock;

UCLASS()
class REALHW09_API UMyTimeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	void SetTimeText(const FString& NewText);
};
