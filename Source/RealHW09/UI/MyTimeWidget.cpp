#include "UI/MyTimeWidget.h"
#include "Components/TextBlock.h"

void UMyTimeWidget::SetTimeText(const FString& NewText)
{
	if (TimeText)
	{
		TimeText->SetText(FText::FromString(NewText));
	}
}