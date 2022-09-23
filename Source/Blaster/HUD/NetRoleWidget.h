// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "NetRoleWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UNetRoleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetRoleText;
	
	void SetText(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetNetRole(APawn* InPawn);
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
};


