// Fill out your copyright notice in the Description page of Project Settings.


#include "NetRoleWidget.h"

void UNetRoleWidget::SetText(FString Text)
{
	if(NetRoleText)
	{
		NetRoleText->SetText(FText::FromString(Text));
	}
	
}

void UNetRoleWidget::SetNetRole(APawn* InPawn)
{
	if(InPawn)
	{
		FString NetRoleString;
		ENetRole Role = InPawn->GetLocalRole();
		switch (Role)
		{
		case ENetRole::ROLE_Authority:
			NetRoleString = TEXT("Authority");
			break;
		case ENetRole::ROLE_AutonomousProxy:
			NetRoleString = TEXT("Autonomous");
			break;
		case ENetRole::ROLE_SimulatedProxy:
			NetRoleString = TEXT("Simulated");
			break;
		case ENetRole::ROLE_None:
			NetRoleString = TEXT("NONE");
			break;
		default:
			NetRoleString = TEXT("NONE_");
			
		}
		SetText(NetRoleString);
	}
}
void UNetRoleWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}