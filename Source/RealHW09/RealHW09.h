#pragma once

#include "CoreMinimal.h"

class MyFunctionLibrary
{
public:
	static void MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay=1.f, FColor InColor=FColor::Cyan)
	{
		if (IsValid(GEngine) && IsValid(InWorldContextActor))
		{
			if (InWorldContextActor->GetNetMode()==NM_Client || InWorldContextActor->GetNetMode()==NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
			}
		}
	}
};