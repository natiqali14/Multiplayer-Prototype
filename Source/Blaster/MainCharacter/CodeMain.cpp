// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMain.h"

#include "Blaster/Weapons/Weapon.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACodeMain::ACodeMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Spring Arm Component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->bUsePawnControlRotation = true;

	// Camera Component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Over HEad UI
	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Over Head Widget"));
	OverHeadWidget->SetupAttachment(GetRootComponent());
	// Movement Settings
	SensController = 1.f;

	// Crouching
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Combat Component
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ACodeMain::BeginPlay()
{
	Super::BeginPlay();
	
}
void ACodeMain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACodeMain,OverlappedWeapon,COND_OwnerOnly)
}
// Called every frame
void ACodeMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}



// Called to bind functionality to input
void ACodeMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Equip",IE_Pressed,this,&ACodeMain::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ACodeMain::CrouchButtonPressed);
	
	PlayerInputComponent->BindAxis("MoveForward",this,&ACodeMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ACodeMain::MoveRight);
	PlayerInputComponent->BindAxis("Lookup", this,&ACodeMain::Lookup);
	PlayerInputComponent->BindAxis("Turn",this,&ACodeMain::Turn);

}

void ACodeMain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->CodeMainRef = this;
	}
}


/**
 * Movement Functions
 */
void ACodeMain::MoveRight(float Value)
{
	if(GetController() && Value != 0)
	{
		const FRotator CurrentRotation = GetController()->GetControlRotation();
		FRotator YawRotation = FRotator(0.f,CurrentRotation.Yaw,0.f);
		FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction,Value);
		
	}
}

void ACodeMain::MoveForward(float Value)
{
	if(GetController() && Value != 0)
	{
		const FRotator CurrentRotation = GetController()->GetControlRotation();
		FRotator YawRotation = FRotator(0.f,CurrentRotation.Yaw,0.f);
		FVector Direction (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction,Value);
		
	}
}

void ACodeMain::Lookup(float Value)
{
	AddControllerPitchInput(Value * SensController );
}

void ACodeMain::Turn(float Value)
{
	AddControllerYawInput(Value * SensController);
}
/**
 *Weapons
 */
void ACodeMain::SetOverlappedWeapon(AWeapon* Weapon)
{
	
	
	if(OverlappedWeapon)
	{
		
		if(IsLocallyControlled())
		{
			OverlappedWeapon->ShowPickupWidget(false);
		}
		
	}
	
	if(IsLocallyControlled())
	{
		if(Weapon)
		{
			
			Weapon->ShowPickupWidget(true);
			
		}
		
	}
	
	
		OverlappedWeapon = Weapon;
	
	  
	  
	
	
}

void ACodeMain::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	UE_LOG(LogTemp,Warning,TEXT("CLIENT"))
	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
	if(OverlappedWeapon)
	{
	
		OverlappedWeapon->ShowPickupWidget(true);
	}
	
}
/**
 *Weapon Equipping
 */
// Equip button pressed
void ACodeMain::EquipButtonPressed()
{
	if(OverlappedWeapon)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(OverlappedWeapon);
		}
		
		else
		{
			ServerEquipWeaponBtnPressed();
		}
	}
}

bool ACodeMain::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}


void ACodeMain::ServerEquipWeaponBtnPressed_Implementation()
{
	if(OverlappedWeapon)
	{
		Combat->EquipWeapon(OverlappedWeapon);
	}
}
// Crouching
void ACodeMain::CrouchButtonPressed()
{
	if(!bIsCrouched)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}
