// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerSessionSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(VisibleAnywhere)
	UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
	UPROPERTY(EditAnywhere,Category = "Multiplayer Hosting Settings", meta = (AllowPrivateAccess = "true"))
	int32 NumberOfConnections;
	UPROPERTY(EditAnywhere,Category = "Multiplayer Hosting Settings", meta = (AllowPrivateAccess = "true"))
	FString MatchType;
	UPROPERTY(EditAnywhere,Category = "Multiplayer Hosting Settings", meta = (AllowPrivateAccess = "true"))
	FString PathToLobby;
	
	
protected:
	virtual  bool   Initialize() override;
public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 MaxNumConn = 4,FString TypeOfMatch = FString(TEXT("FreeForAllByFrio")),
		FString LobbyPath =FString(TEXT("/Game/ThirdPerson/Maps/Lobby2")) );
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
	void MenuTearDown();
	virtual  void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnJoinSession( EOnJoinSessionCompleteResult::Type Result);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccesful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	
};
