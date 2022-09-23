// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

#include "OnlineSubsystem.h"
#include "Components/Button.h"


bool UMenu::Initialize()
{
	
	if(!Super::Initialize())
	{
		return false;
	}

	if(HostButton)
	{
		HostButton->OnClicked.AddDynamic(this,&ThisClass::HostButtonClicked);
	}
	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this,&ThisClass::JoinButtonClicked);
	}
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSession.AddDynamic(this,&ThisClass::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSession.AddDynamic(this,&ThisClass::OnStartSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySession.AddDynamic(this,&ThisClass::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSession.AddUObject(this,&ThisClass::OnFindSession);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSession.AddUObject(this,&ThisClass::OnJoinSession);
	}
	

	
	return true;
}

void UMenu::MenuSetup(int32 MaxNumConn,FString TypeOfMatch,FString LobbyPath)
{
	NumberOfConnections = MaxNumConn;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"),*LobbyPath);
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		FInputModeUIOnly InputModeUIOnly;
		InputModeUIOnly.SetWidgetToFocus(TakeWidget());
		InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeUIOnly);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	UE_LOG(LogTemp,Warning,TEXT("EZPZPZP"))
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->CreateSession(NumberOfConnections,MatchType);
	}
}

void UMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	UE_LOG(LogTemp,Warning,TEXT("EZPZPZP"))
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(10000);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		FInputModeGameOnly GameOnly;
		PlayerController->SetInputMode(GameOnly);
		PlayerController->SetShowMouseCursor(false);
	}
}

void UMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		GetWorld()->ServerTravel(PathToLobby);
		
	}
	HostButton->SetIsEnabled(true);
;}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::UnknownError)
	{
		return;
	}
	IOnlineSessionPtr OnlineSessionInterface;
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		 OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}
	if(!OnlineSessionInterface.IsValid()) return;
	FString Address;
	bool bSuccess = OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession,Address);
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if(PlayerController && bSuccess)
	{
		PlayerController->ClientTravel(Address,ETravelType::TRAVEL_Absolute);
	}
}

void UMenu::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccesful)
{
	if(bWasSuccesful)
	{
		if(MultiplayerSessionSubsystem)
		{
			int32 Total = SessionResult.Num();
			
			
			
			for (auto Result : SessionResult)
			{
				FString Match;
				Result.Session.SessionSettings.Get(FName("MatchType"),Match);
				if(GEngine)
				{
					FString Name = Result.Session.OwningUserName;
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1,100,FColor::Blue,
							FString::Printf(TEXT("Session Name : %s"),*Name));
					}
					
					if(Result.Session.SessionSettings.Get(FName("MatchType"),Match)){
						if(MatchType == Match)
						{
							MultiplayerSessionSubsystem->JoinSessions(Result);
							return;
						}
					}
				}
			}
		}
	}
	JoinButton->SetIsEnabled(true);
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}
