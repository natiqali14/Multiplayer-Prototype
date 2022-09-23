// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeMain.h"
#include "Animation/AnimInstance.h"
#include "CodeMainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UCodeMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Ref", meta = (AllowPrivateAccess = "true"))
	ACodeMain* CodeMainRef;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Properties", meta = (AllowPrivateAccess = "true"))
	float MainCharacterSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Properties", meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Main Character | Properties", meta = (AllowPrivateAccess = "true"))
	bool bCrouched = false;
};


