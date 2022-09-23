// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSession, bool , bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSession, const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccesful)
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSession, EOnJoinSessionCompleteResult::Type Result)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSession, bool , bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySession, bool , bWasSuccessful);



/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	IOnlineSessionPtr OnlineSessionInterface;
	//
	// Delegates to be added in Online subsystem interface...
	// Use for Internal Callbacks
	//
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastOnlineSessionSettings;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	int32 LastNumPublicCon;
	FString LastMatchType;
	bool bDestroySessionCalled = false;
protected:
	//
	// Internal callbacks bind with Delegates from Online Subsystem interface
	//
	void OnCreateSessionComplete(FName SessionName,bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(FName SessionName,bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName,bool bWasSuccessful);
	
public:
	UMultiplayerSessionSubsystem();
	//
	// Delegate for Comm. between Menu class and Subsystem class
	//
	FMultiplayerOnCreateSession MultiplayerOnCreateSession;
	FMultiplayerOnDestroySession MultiplayerOnDestroySession;
	FMultiplayerOnFindSession MultiplayerOnFindSession;
	FMultiplayerOnJoinSession MultiplayerOnJoinSession;
	FMultiplayerOnStartSession MultiplayerOnStartSession;
	
	//
	// To handle session functionality. The menu class will call these
	//
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSessions(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();
	
	
};
