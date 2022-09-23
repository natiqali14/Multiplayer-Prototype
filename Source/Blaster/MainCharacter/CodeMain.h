// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "CodeMain.generated.h"

UCLASS()
class BLASTER_API ACodeMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACodeMain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
private:
	/** Camera for Character */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** SpringArm for Character */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Mouse Sensitivity Controller */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Movement Setting | Mouse Senstivity", meta = (AllowPrivateAccess = "true"))
	float SensController;

	/** Over Head Widget for user */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI | Over Head", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverHeadWidget;

	/** OverlappedWeapon */
	UPROPERTY(VisibleAnywhere,Category = "Weapon", ReplicatedUsing=OnRep_OverlappingWeapon)
	class AWeapon* OverlappedWeapon;

	/** Combat Component */
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	/** RPC for Equipping on client */
	UFUNCTION(Server,Reliable)
	void ServerEquipWeaponBtnPressed();




public:
	/**
	 * Movement Functions
	 */
	void MoveRight(float Value);
	void MoveForward(float Value);
	void Lookup(float Value);
	void Turn(float Value);

	/**
	 *Weapon
	 */
	void SetOverlappedWeapon(AWeapon* Weapon);
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// Equipping Weapon
	void EquipButtonPressed();
	bool IsWeaponEquipped();
	//Crouching
	void CrouchButtonPressed();

	// Aiming
	bool IsAiming();
};
