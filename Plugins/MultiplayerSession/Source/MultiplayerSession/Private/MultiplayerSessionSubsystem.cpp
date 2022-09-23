// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"



UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem():
CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionComplete)),
FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionComplete)),
JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnJoinSessionComplete)),
StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnStartSessionComplete)),
DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&ThisClass::OnDestroySessionComplete))
{
	 IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnCreateSession.Broadcast(false);
		return;
	}
	if(const auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession); ExistingSession != nullptr)
	{
		LastMatchType = MatchType;
		LastNumPublicCon = NumPublicConnections;
		bDestroySessionCalled = true;
		DestroySession();
	
	}
	CreateSessionCompleteDelegateHandle =
		OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastOnlineSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastOnlineSessionSettings->bAllowInvites = true;
	LastOnlineSessionSettings->bIsLANMatch = false;
	LastOnlineSessionSettings->bUsesPresence = true;
	LastOnlineSessionSettings->bAllowJoinViaPresence = true;
	LastOnlineSessionSettings->bAllowJoinInProgress = true;
	LastOnlineSessionSettings->bShouldAdvertise = true;
	LastOnlineSessionSettings->NumPublicConnections = NumPublicConnections;
	LastOnlineSessionSettings->bUseLobbiesIfAvailable = true;
	//LastOnlineSessionSettings->BuildUniqueId = 1;
	LastOnlineSessionSettings->Set(FName("MatchType"),MatchType,
		EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	if(ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();LocalPlayer)
	{
		if(!OnlineSessionInterface->CreateSession
			(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession, *LastOnlineSessionSettings))
		{
			OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
			MultiplayerOnCreateSession.Broadcast(false);
		}
		
		
	}
	
}
void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnCreateSession.Broadcast(false);
	}
	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	if(bWasSuccessful)
	{
		MultiplayerOnCreateSession.Broadcast(true);
		
		FString Match;
		LastOnlineSessionSettings->Get(FName("MatchType"),Match);
		
	}
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResults)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnFindSession.Broadcast(TArray<FOnlineSessionSearchResult>(),false);
		return;		
	}
	
	FindSessionsCompleteDelegateHandle =
		OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
	OnlineSessionSearch->bIsLanQuery = false;
	OnlineSessionSearch->MaxSearchResults = MaxSearchResults;
	OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);

	if(ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();LocalPlayer)
	{
		if(!OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),
			OnlineSessionSearch.ToSharedRef()))
			
		{
			OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
			MultiplayerOnFindSession.Broadcast(TArray<FOnlineSessionSearchResult>(),false);
		}
		
		
	}
}
void UMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnFindSession.Broadcast(TArray<FOnlineSessionSearchResult>(),false);
		return;
	}
	if(bWasSuccessful)
	{
		
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSession.Broadcast(OnlineSessionSearch->SearchResults,true);
	}
	
}
void UMultiplayerSessionSubsystem::JoinSessions(const FOnlineSessionSearchResult& SessionResult)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	
	JoinSessionCompleteDelegateHandle =
		OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(LocalPlayer)
	{
		bool Success =
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(),NAME_GameSession,SessionResult);
		if(!Success)
		{
			OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);			
			MultiplayerOnJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		}
	
	}
}
void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	
	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	MultiplayerOnJoinSession.Broadcast(Result);
}


void UMultiplayerSessionSubsystem::DestroySession()
{
	if(!OnlineSessionInterface.IsValid())
	{
		MultiplayerOnDestroySession.Broadcast(false);
		return;
	}
	DestroySessionCompleteDelegateHandle =
		OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
	if(!OnlineSessionInterface->DestroySession(NAME_GameSession))
	{
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySession.Broadcast(false);
	}
	
	
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		return;
	}
	if(OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if(bDestroySessionCalled)
	{
		
		CreateSession(LastNumPublicCon,LastMatchType);
		MultiplayerOnDestroySession.Broadcast(true);
	}
}

void UMultiplayerSessionSubsystem::StartSession()
{
}





void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

